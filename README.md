# FreshContainer
Basic implementation of a container  in C

### Usage

Before testing the program you must download a basic Alpine Linux filesystem which is used as the filesystem for the jailed process.
It can be downloaded by executing the download script located in this directory using the following command on a Linux terminal:

```bash
./download_filesystem.sh
```

After that look for a directory named root inside this directory with the command ```ls ./root```,
if you found it then the download was probably successful.

Then you can compile the program using ```make```.

For this program to run successfully it will require super user privileges since some of the system call used (with some flags) depends on super
user privileges to execute successfully.

Run the program using:

```bash
sudo ./container
```
