cmd_/home/.s2xxx/.git/edu/lab/lab1.mod := printf '%s\n'   lab1.o | awk '!x[$$0]++ { print("/home/.s2xxx/.git/edu/lab/"$$0) }' > /home/.s2xxx/.git/edu/lab/lab1.mod
