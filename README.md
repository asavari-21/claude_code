# AI CLI Agent (C++)

A lightweight command-line AI agent written in C++ that interacts with an LLM via HTTP and supports tool-based execution.

## Features

- Chat with an LLM using a prompt (`-p` flag)
- Persistent conversation loop (agent-style interaction)
- Built-in tool support:
  - **Read** – Read file contents
  - **Write** – Create or overwrite files
  - **Bash** – Execute shell commands

## How It Works

The program:
1. Sends user input to an LLM API
2. Receives responses (including tool calls)
3. Executes requested tools locally
4. Feeds results back to the model
5. Repeats until a final response is generated

## Requirements

- C++17+
- `cpr` (HTTP client)
- `nlohmann/json`
- OpenRouter API key

## Setup

```bash
export OPENROUTER_API_KEY="your_api_key_here"
