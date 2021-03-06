## epiClassifier

### Author
© 2011  [Philippe Ameline](https://twitter.com/p_ameline)

epiClassifier's original code is part of [Episodus](https://sourceforge.net/projects/episodus/), an innovative open source Electronic Health Record app based on knowledge management technologies able to tell a patient's health (hi)story over time and to organize it like a project manager.

## License
Free and open source license: Apache License 2.0

### About
ICPC semi-automatic classification engine for GNU/Linux OS.

ICPC is the International Classification of Primary Care.

### Try it

#### Original version online 
[promethe.org](http://promethe.org) is based on the original version, prior to the modifications recorded on this git

#### Current version
The current version is not yet ready for an online demo. If you want to test it and/or contribute to the development, you can build it yourself and use the default credentials giving access to the online database.

#### Database
You can use the online test database with the default credentials from config/sql.cfg.sample (you need to rename this file to sql.cfg). Don't use real medical data with this test database as it cannot handle TLS encryption yet. If the database is down, please let us know.

### Future

* We are coding a RESTful API based on Casablanca C++ REST SDK that will allow any web based or standalone software running on any OS to use epiClassifier as a service
* epiClassifier will be packaged for Debian & Ubuntu
* Anyone willing to package epiClassifier for a free software distribution is welcome, we will provide help on a best effort basis
* This is free software, you are free to build it for any OS you wish without limitation, but we might not be available to help you deal with proprietary operating systems

### Build
* [Original build instructions](http://philippe.ameline.free.fr/epiClassifier/instructions.htm)
* [Build instructions adapted for Debian Stretch](BUILD.md)

### Databases

epiClassifier relies on a MySQL/MariaDB database composed of several tables:

* 3BT: Bilingual Biclassified Belgian Terminology (License: unknown) This thesaurus contains clinicals labels in French and Dutch linked to ICPC-2 and ICD-10.
* Episodus ontology (License: open source)
* ICPC
* ICD-10

We are in the process of securing their release under open source licences.

### Contact

[Jérôme Pinguet](https://twitter.com/medecinelibre)

### References
#### English
* [Letrilliart, Laurent, Bénédicte Gelas-Dore, Bernard Ortolan, and Cyrille Colin. “Prometheus: The Implementation of Clinical Coding Schemes in French Routine General Practice.” Journal of Innovation in Health Informatics 14, no. 3 (2006): 157–165.](http://hijournal.bcs.org/index.php/jhi/article/view/626)
* [Letrilliart, Laurent, et. al, “Automatic Coding of Reasons for Hospital Referral from General Medicine Free-Text Reports. - PubMed - NCBI.” Accessed January 26, 2017. https://www.ncbi.nlm.nih.gov/pubmed/11079931.](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC2243796/)
#### French
* [Ameline, Philippe. “Moteur CISP.” presented at the 15èmes ateliers du CISP Club, Bruxelles, 2012.](http://www.cispclub.org/cispclub/docs/27/000256/0000316.pdf)
