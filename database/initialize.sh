mysql -u root -p${MYSQL_ROOT_PASSWORD} -e "CREATE DATABASE IF NOT EXISTS ontology"
mysql -u root -p${MYSQL_ROOT_PASSWORD} -e "CREATE USER 'ontology'@'%' IDENTIFIED BY 'ontology'"  
mysql -u root -p${MYSQL_ROOT_PASSWORD} -e "GRANT SELECT ON ontology.* TO 'ontology'@'%'"
mysql -u root -p${MYSQL_ROOT_PASSWORD} -D ontology < /usr/sql/ontology.sql

