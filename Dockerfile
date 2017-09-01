FROM sensorlab6/vesna-tools

ARG ghtoken

# File Author / Maintainer
MAINTAINER Tomaz Solc

# Set debian frontend
ENV DEBIAN_FRONTEND=noninteractive

# Compile firmware
WORKDIR /root
RUN git clone -b logatec-3 --depth 1 https://$ghtoken@github.com/avian2/vesna-drivers.git

WORKDIR /root/logatec-temp-monitor
COPY firmware firmware

RUN	cd firmware && \
	make temp-monitor.out

# Install measurement script and dependencies
WORKDIR /root
RUN	apt-get install -y python-pip python-numpy python-lxml && \
	git clone --depth 1 https://github.com/sensorlab/sna-lgtc-support.git && \
	pip install sna-lgtc-support/videk-client vesna-alhtools

WORKDIR /root/logatec-temp-monitor
COPY measure.py measure.py

# This script uploads firmware and starts the measurement script
COPY start.sh start.sh
RUN chmod 755 start.sh

ENTRYPOINT ["/root/logatec-temp-monitor/start.sh"]
