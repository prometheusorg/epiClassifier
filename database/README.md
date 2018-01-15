## database setup

epiClassifier/database contains an SQL dump of the ontology database that is
required to run the coding assistant.

You can launch build and run a Docker container to deploy the database with MariaDB locally or
on a remote server.

If you follow this example, your MariaDB server will listen on port 3307.

Replace the root password my-password by a real password.

The proposed Dockerfile creates a MariaDB user 'ontology' with password 'ontology'
and SELECT permission.

git clone https://github.com/prometheusorg/epiClassifier.git
cd epiClassifier/database
docker build -t ontology .
docker run -d -p 3307:3306 --restart unless-stopped -e MYSQL_ROOT_PASSWORD=my-password ontology
