#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc < 3 || std::string(argv[1]) != "-p") {
        std::cerr << "Expected first argument to be '-p'" << std::endl;
        return 1;
    }

    std::string prompt = argv[2];

    if (prompt.empty()) {
        std::cerr << "Prompt must not be empty" << std::endl;
        return 1;
    }

    const char* api_key_env = std::getenv("OPENROUTER_API_KEY");
    const char* base_url_env = std::getenv("OPENROUTER_BASE_URL");

    std::string api_key = api_key_env ? api_key_env : "";
    std::string base_url = base_url_env ? base_url_env : "https://openrouter.ai/api/v1";

    if (api_key.empty()) {
        std::cerr << "OPENROUTER_API_KEY is not set" << std::endl;
        return 1;
    }

    json messages = json::array({
        {
            {"role", "user"},
            {"content", prompt}
        }
    });

    while (true){        
        json request_body = {
            {"model", "anthropic/claude-haiku-4.5"},
            {"messages", messages},
            {"tools", json::array({
                {
                    {"type", "function"},
                    {"function", {
                        {"name", "Read"},
                        {"description", "Read and return contents of a file"},
                        {"parameters", {
                            {"type", "object"},
                            {"properties", {
                                {"file_path", {
                                    {"type", "string"},
                                    {"description", "Path to the file to read"}
                                }}
                            }},
                            {"required", json::array({"file_path"})}
                        }}                            
                    }}
                }
            })}
        };

        cpr::Response response = cpr::Post(
            cpr::Url{base_url + "/chat/completions"},
            cpr::Header{
                {"Authorization", "Bearer " + api_key},
                {"Content-Type", "application/json"}
            },
            cpr::Body{request_body.dump()}
        );
        
        if (response.status_code != 200) {
            std::cerr << "HTTP error: " << response.status_code << std::endl;
            return 1;
        }

        json result = json::parse(response.text);

        auto message = result["choices"][0]["message"];
        messages.push_back(message);

        if (message.contains("tool_calls") && !message["tool_calls"].empty()){
            auto tool_call = message["tool_calls"][0];

            std::string tool_call_id = tool_call["id"];
            std::string function_name = tool_call["function"]["name"];
            std::string arguments_str = tool_call["function"]["arguments"];

            json args = json::parse(arguments_str);

            std::string result_content;

            if (function_name == "Read"){
                std::string file_path = args["file_path"];

                std::ifstream file(file_path);
                if (!file.is_open()){
                    result_content = "Failed to open file\n";
                } else{
                    std::string line;
                    while (std::getline(file, line)){
                        result_content += line +  "\n";
                    }
                }
            }

            messages.push_back({
                {"role", "tool"},
                {"tool_call_id", tool_call_id},
                {"content", result_content}
            });

        } else {
            if(message.contains("content") && !message["content"].is_null()){
                std::cout << message["content"].get<std::string>() << std::endl;
            }
            break;
        }
    }    

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    // std::cerr << "Logs from your program will appear here!" << std::endl;

    // std::cout << result["choices"][0]["message"]["content"].get<std::string>();

    return 0;
}