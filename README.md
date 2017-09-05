## epiClassifier

### Author
© 2011  Philippe Ameline

epiClassifier's original code is part of [Episodus](https://sourceforge.net/projects/episodus/), an innovative open source Electronic Health Record app based on knowledge management technologies able to tell a patient's health (hi)story over time and to organize it like a project manager.

## License
Free and open source license: Apache License 2.0

### About
ICPC semi-automatic classification engine for GNU/Linux OS.

ICPC is the International Classification of Primary Care.

### Try it


### Future

* We are coding a SOAP API based on gSOAP that will allow any web based or standalone software running on any OS to use epiClassifier as a service
* epiClassifier will be packaged for Debian & Ubuntu
* Anyone willing to package epiClassifier for a free software distribution is welcome

### Build
* [Original build instructions](http://philippe.ameline.free.fr/epiClassifier/instructions.htm)
* [Build instructions adapted for Debian Stretch](BUILD.md)

### Databases

epiClassifier relies on a MySQL/MariaDB database composed of several tables:

* 3BT: Bilingual Biclassified Belgian Terminology (License: unknown)
* Episodus ontology (License: open source)
* ICPC
* ICD-10

We are in the process of securing their release under open source licences. Until then, you can contact us to get the databases for development or research purposes.

### Contact

[Jérôme Pinguet](https://twitter.com/medecinelibre)

### References
* [Letrilliart, Laurent, Bénédicte Gelas-Dore, Bernard Ortolan, and Cyrille Colin. “Prometheus: The Implementation of Clinical Coding Schemes in French Routine General Practice.” Journal of Innovation in Health Informatics 14, no. 3 (2006): 157–165.](http://hijournal.bcs.org/index.php/jhi/article/view/626)
* [Letrilliart, Laurent, et. al, “Automatic Coding of Reasons for Hospital Referral from General Medicine Free-Text Reports. - PubMed - NCBI.” Accessed January 26, 2017. https://www.ncbi.nlm.nih.gov/pubmed/11079931.](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC2243796/)

