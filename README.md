# TaskForge

## Short description

This program models GitHub's project management system.

## Features

* Create projects, issues and repositories
* Change the structure of the hierarchy and tag certain issues
* Iterate over the hierarchy depending on tags
* Full state-based logic for issues

## Build and run

It is recommended to run this program with Docker. The provided Docker environment contains all tools required to build, run, debug, and investigate the program.

### Using the Docker Hub image

Assuming Docker is installed and running:

```bash
docker pull ethanvaneyden/taskforge:latest
docker run -it --name taskforge --rm ethanvaneyden/taskforge:latest
```

> **Note:** The `-it` flags are required because TaskForge is an interactive CLI application that accepts input from `stdin`.

### Building the image locally

If you want to build the Docker image from the provided Dockerfile:

```bash
docker build -t taskforge .
docker run -it --name taskforge --rm taskforge
```

## Debugging

The Docker image includes GDB with debug symbols enabled during compilation.

To run TaskForge using GDB:

```bash
docker run -it --rm taskforge gdb ./taskforge
```

Alternatively, if using the Docker Hub image:

```bash
docker run -it --rm ethanvaneyden/taskforge:latest gdb ./taskforge
```

## Valgrind

Valgrind is included in the Docker image for memory checking.

To run the program with Valgrind:

```bash
docker run -it --rm taskforge make valgrind
```

Alternatively, using the Docker Hub image:

```bash
docker run -it --rm ethanvaneyden/taskforge:latest make valgrind
```

The `valgrind` Makefile target runs:

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./taskforge
```
## Makefile

The project includes a Makefile with the following targets:

| Target          | Description                                |
| --------------- | ------------------------------------------ |
| `make`          | Builds the TaskForge executable            |
| `make clean`    | Removes generated build and coverage files |
| `make valgrind` | Runs TaskForge with Valgrind               |
| `make coverage` | Generates a code coverage report           |
| `make cleandoc` | Removes generated documentation            |
| `make zip`      | Creates a submission ZIP archive           |

The Docker image contains the required C++ compiler, Make, GDB and Valgrind
