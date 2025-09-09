# minishell

## What it is?

Minishell is a program intended to replicate some of the features of shells such as bash. It contains features such as:

- Built-in commands (ls, echo, cd, export, unset, pwd)
- Running binary files
- Variable expansion/handling
- File redirections
- Handling of pipelines

## How does it work?

This shell works by using the readline function to receive a string of commands to execute, similar to bash, and then parse them into the abstract syntax tree (AST). They are then executed as necessary, usually in child processes, and handles any file redirections as input or output if present.

> [!NOTE]
> This was a group project that I did with [Jules]([https://github.com/Jules478]). I was primarily responsible for parsing while Jules worked on the execution, built-ins, and signal handlers. 

> [!NOTE]
> This is intended as a guide only. Writing and understanding your own code is better than copying code. 
