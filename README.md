# Ship-Shell: A Linux Shell Implementation Project
This README will guide you through the setup and utilization of the Ship-Shell project, a new Linux 
shell built using low-level system calls.
  
# Summary
The Ship-Shell project is a hands-on exploration of the Linux shell environment and low-level system 
calls. The project is built around a new Linux shell dubbed "Ship Shell," which supports several 
built-in commands such as "cd", "path", and "exit". It also allows users to access commands from 
the "/bin" folder, change Path variables, and supports features such as parallelism "&" and 
redirection ">".
  
# Setup

    1. Ensure you are in the correct file directory.
    2. You must be in a Linux environment, or have a Linux subsystem installed on your Windows machine.
    3. To create the executable, simply type make into the command line. You can also use the direct 
       executable provided by typing ./ship-shell into the command line.

# Usage

To start using Ship Shell, follow these steps:

    1. Run the executable by typing ./ship-shell into the command line.
    2. Now, you should be in the Ship Shell environment. Here, you can use the built-in commands like 
    cd, path, exit, and any commands available in the "/bin" folder.
    4. To change the path variable, use the path command followed by the desired path.
    5. The shell supports parallelism with "&". Simply append "&" to the end of your commands to run 
       them in the background.
    6. The shell also supports output redirection with ">". Just append ">" followed by the file name 
       to your command, and the output will be redirected to the specified file.
