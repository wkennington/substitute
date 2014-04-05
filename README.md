# Intro
Substitute is a simple utility for performing string replacement on text files.

# Dependencies
Substitute has a minimal set of dependencies mostly contained in any essential build environment:
- C Compiler
- Make
- Automake
- Autoconf
- PKG-Config
- Check (for testing)

# Building
Building substitute is similar to any other autotools managed project.

```bash
git clone git://github.com/wkennington/substitute
cd substitute

autoreconf -vfi
./configure --prefix="<INSTALL DIR>"
make
make install
```

# Running
To print the help text:
```bash
substitute -h
```

To perform simple replacements of hello -> world and foo -> bar:
```bash
substitute -r hello world -r foo bar infile outfile
```
