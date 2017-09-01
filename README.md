Build:

    $ docker build --build-arg=ghtoken=... -t logatec-temp-monitor .

Run:

    $ docker run -i --privileged -v /etc/videk/api.key:/etc/videk/api.key -v /etc/hostname:/etc/videk/hostname logatec-temp-monitor
