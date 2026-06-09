#!/usr/bin/env bash

if [ "$EUID" -ne 0 ]; then
    echo "Please run as root to install gbs"
    exit 1
fi

# Compile the program
make

# Check if the compilation was successful
if [ ! -f gbs ]; then
  echo "Compilation failed. Please fix the errors and try again."
  exit 1
fi

# Check if is already installed
if [ -f /usr/local/bin/gbs ]; then
  echo "gbs is already installed. Do you want to overwrite it? (y/n)"
  read answer
  if [ "$answer" != "y" ]; then
    echo "Installation cancelled."
    exit 0
  fi
fi

# Install the binary to /usr/local/bin
cp gbs /usr/local/bin/gbs

echo "gbs has been installed successfully!"
echo "You can now run 'gbs' from the command line."