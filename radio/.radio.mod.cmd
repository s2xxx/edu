cmd_/home/.s2xxx/.git/edu/radio/radio.mod := printf '%s\n'   radio_main.o radio_or.o | awk '!x[$$0]++ { print("/home/.s2xxx/.git/edu/radio/"$$0) }' > /home/.s2xxx/.git/edu/radio/radio.mod
