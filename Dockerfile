FROM phusion/baseimage

RUN rm -f /etc/service/sshd/down
RUN sed -ri 's/^#?PermitRootLogin\s+.*/PermitRootLogin yes/' /etc/ssh/sshd_config

RUN groupadd ciscn && \
	useradd -g ciscn ciscn -m && \
	password=$(openssl passwd -1 -salt 'abcdefg' '123456') && \
	sed -i 's/^ciscn:!/ciscn:'$password'/g' /etc/shadow


RUN apt-get update &&\
        apt-get install -y php apache2 libapache2-mod-php 

RUN apt-get install -y php-dev
RUN apt-get install -y pandoc

COPY pmarkdown /tmp/pmarkdown

RUN cd /tmp/pmarkdown && \
        phpize &&\
        ./configure --with-php-config=/usr/bin/php-config && \
        make clean && make && make install && \
        rm -rf /tmp/pmarkdown

COPY src /var/www/html
ADD php.ini /etc/php/7.0/apache2/
ADD 20-pmarkdown.ini /etc/php/7.0/apache2/conf.d
ADD apache2.conf /etc/apache2/

ADD readflag /readflag
ADD ./flag /
RUN chmod a-r /flag && chmod 755 /readflag  && chmod u+s /readflag && chmod 777 /var/www/html/posts


ADD ./start.sh /etc/my_init.d/
RUN chmod u+x /etc/my_init.d/start.sh && rm -rf /var/www/html/index.html


CMD /etc/my_init.d/start.sh
