### Git Interactive Branch Switcher

A very simple interactive branch switcher for Git repositories, to quickly switch between branches without having to type the branch name.

### How to install

1. Clone the repository:

   ```bash
   git clone https://github.com/Axthauvin/gbs
   ```

2. Start the install script:

   ```bash
   cd gbs
   ./install.sh
   ```

3. The program should now be available globally as `gbs`, and you can delete the cloned repository if you want.

### Local development

1. Compile the program:
   ```bash
   make
   ```
2. Run the program in a Git repository:
   ```bash
   ./gbs
   ```

# TODO

- [x] Be able to search for branches by typing their name
- [ ] Menu to create a new branch
