cmd_/home/.s2xxx/.git/edu/param/param.mod := printf '%s\n'   param.o | awk '!x[$$0]++ { print("/home/.s2xxx/.git/edu/param/"$$0) }' > /home/.s2xxx/.git/edu/param/param.mod
