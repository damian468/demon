# Daemon File Scanner

This project implements a daemon program that recursively scans the file system for files or directories matching specified filename fragments. The results are logged to syslog, and the program can handle specific signals for immediate scanning and verbosity options.

## Features

- Recursively scans the file system for matching files and directories.
- Logs results to syslog.
- Handles `SIGUSR1` signal for immediate scanning.
- Supports verbosity options for detailed logging.

## File Structure

```
daemon-program
├── src
│   ├── main.c          # Entry point of the daemon program.
│   ├── daemon.c        # Implementation of the daemon functionality.
│   ├── daemon.h        # Header file for daemon functions.
│   ├── file_scanner.c  # Logic for recursive file scanning.
│   ├── file_scanner.h  # Header file for file scanning functions.
│   ├── signal_handler.c # Implementation of signal handling.
│   ├── signal_handler.h # Header file for signal handling functions.
│   └── logger.c        # Logging functionality implementation.
│       └── logger.h    # Header file for logging functions.
├── Makefile             # Build instructions for the project.
└── README.md            # Documentation for the project.
```

## Compilation

To compile the project, navigate to the `daemon-program` directory and run:

```
make
```

This will generate the executable for the daemon program.

## Running the Daemon

To run the daemon, use the following command:

```
./daemon_program [options] <filename_fragments>
```

### Options

- `-v` : Enable verbose logging.
- `-h` : Display help information.

## Signal Handling

- Send `SIGUSR1` to the daemon process to trigger an immediate scan.

## Logging

Logs are written to syslog. The verbosity of the logs can be adjusted using command-line options.

## License

This project is licensed under the MIT License.