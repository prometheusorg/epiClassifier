FROM mariadb:latest
# Author
MAINTAINER jerome

# Environment variables
ENV MYSQL_DATABASE ontology

RUN mkdir /usr/sql
#RUN chmod 644 /usr/sql

ADD ontology.sql /usr/sql/ontology.sql
ADD initialize.sh /docker-entrypoint-initdb.d/initialize.sh
