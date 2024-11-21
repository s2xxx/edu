cmd_/home/.s2xxx/.git/edu/radio/radio_or.mod := printf '%s\n'   radio_or.o | awk '!x[$$0]++ { print("/home/.s2xxx/.git/edu/radio/"$$0) }' > /home/.s2xxx/.git/edu/radio/radio_or.mod
