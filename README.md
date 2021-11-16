# Αναζήτηση και συσταδοποίηση διανυσμάτων στη C++ 

## Μέλη ομάδας
Αλέξανδρος Λινάρδος (sdi1600093@di.uoa.gr) <br>
Αντώνης Μήλιος (sdi1600100@di.uoa.gr) <br>
Github repo: https://github.com/AlexLinardos/Project1-LSH-and-Clustering

## Περιγραφή project
Δοθέντος ενός συνόλου δεδομένων που αποτελείται από πολυδιάστατα διανύσματα, υλοποιούμε κατάλληλο κώδικα για τα εξής:
1. Εύρεση κατά προσέγγιση κοντινότερων γειτόνων ενός διανύσματος (query) με τη χρήση των τεχνικών "LSH (Locality Sensitive Hashing)" και "Προβολής σε Υπερκύβο".
2. Συσταδοποίηση των δεδομένων με τρείς διαφορετικές μεθόδους του αλγορίθμου k-means: Lloyd's algorithm, Reverse Assignment with LSH Range Search και Reverse Assignment with Hypercube Range Search. Η απόδοσή τους αξιολογείται με την μετρική Silhouette.

## Αρχεία κώδικα / επικεφαλίδων
<b>Makefile > </b> Αρχείο makefile που χρησιμοποιείται για τη μεταγλώττιση των προγραμμάτων.

<b>cluster.conf > </b> Ενδεικτικό αρχείο όπου ορίζονται οι παράμετροι της συσταδοποίησης (clustering). Μετά τις άνω-κάτω τελείες και πριν την εκάστοτε αριθμητική τιμή πρέπει να υπάρχει κενό για να διαβαστεί σωστά ένα configuration file.

<b>LSH > </b> Φάκελος που περιέρχει τα εξής αρχεία για την υλοποίηση του ΑΝΝ με Locality Sensitive Hashing <br>
* LSH.cpp : Βασικό αρχείο κώδικα για την εκτέλεση του αλγορίθμου ANN με τη μέθοδο LSH.
* LSH_ui.hpp : Αρχείο επικεφαλίδας με κατάλληλες δομές και συναρτήσεις για το διάβασμα και την αποθήκευση των παραμέτρων της μεθόδου LSH(με command line arguments ή χωρίς).
* LSH.hpp : Αρχείο επικεφαλίδας για την υλοποίηση δομής που περιλαμβάνει τις συναρτήσεις και τους πίνακες κατακερματισμού που απαιτεί η μέθοδος LSH, καθώς και τις συναρτήσεις αναζήτησης πλησιέστερων γειτόνων.

<b>Hypercube > </b> Φάκελος που περιέρχει τα εξής αρχεία για την υλοποίηση του ΑΝΝ με Προβολή σε Υπερκύβο <br>
* Cube.cpp : Βασικό αρχείο κώδικα για την εκτέλεση του αλγορίθμου ANN με τη μέθοδο προβολής σε υπερκύβο.
* Cube_ui.hpp : Αρχείο επικεφαλίδας με κατάλληλες δομές και συναρτήσεις για το διάβασμα και την αποθήκευση των παραμέτρων της μεθόδου προβολής σε υπερκύβο (με command line arguments ή χωρίς).
* Cube.hpp : Αρχείο επικεφαλίδας για την υλοποίηση δομής που περιλαμβάνει τις συναρτήσεις και τον πίνακα κατακερματισμού που απαιτεί η μέθοδος LSH, καθώς και τις συναρτήσεις αναζήτησης πλησιέστερων γειτόνων.

<b>Clustering > </b> Φάκελος που περιέρχει τα εξής αρχεία για την υλοποίηση της συσταδοποίησης δεδομένων <br>
* Clustering.cpp : Βασικό αρχείο κώδικα για την εκτέλεση του αλγορίθμου ANN με μέθοδο που ορίζεται από το command line argument "-method".
* Clustering_ui.hpp : Αρχείο επικεφαλίδας με κατάλληλες δομές και συναρτήσεις για το διάβασμα και την αποθήκευση των παραμέτρων συσταδοποίησης, οι οποίες δίνονται στο πρόγραμμα από τον χρήστη ως command line arguments.<br>
* Clustering.hpp : Αρχείο επικεφαλίδας για την υλοποίηση δομής που περιλαμβάνει όλες τις αναγκαίες δομές για την αναπαράσταση των συστάδων, καθώς και όλες τις συναρτήσεις για την υλοποίηση των τριών μεθόδων συσταδοποίησης k-means που αναφέρθηκαν παραπάνω. Περιέχει επίσης τις συναρτήσεις για την αξιολόγηση της συσταδοποίησης μέσω της μετρικής Silhouette.

<b>misc > </b> Φάκελος που περιέρχει διάφορα αρχεία επικεφαλίδας τα οποία χρησιμοποιούνται από όλες τις μεθόδους <br>
* hashing.hpp : Αρχείο επικεφαλίδας που περιέχει την υλοποίηση της οικογένειας συναρτήσεων κατακερματισμού H, η οποία χρησιμοποιείται και από τις δύο μεθόδους ANN.
* utilities.hpp : Αρχείο επικεφαλίδας που περιέχει συναρτήσεις γενικής χρήσεως, καθώς και τη βασική δομή Item που χρησιμοποιείται για την δημιουργία των σημείων/διανυσμάτων ως αντικείμενα του προγράμματος.

<b>datasets > </b> Φάκελος που περιέχει ενδεικτικά input/query files. <br>
* input_small_id
* query_small_id

## Οδηγίες μεταγλώττισης
Για τη μεταγλώττιση του προγράμματος αρκεί η εκτέλεση της εντολής <b>make</b>.

## Οδηγίες εκτέλεσης
Μετά τη μεταγλώττιση μπορούμε να εκτελέσουμε τον αλγόριθμο που θέλουμε ως εξής:
* LSH: Είτε χωρίς command line arguments (`./bin/LSH`), όποτε θα καθοδηγηθούμε από το in-program interface για την αρχικοποίηση των αναγκαίων παραμέτρων, είτε με command line arguments ακολουθώντας τη μορφή `./bin/LSH -i <input file> -q <query file> -k <int> -L <int> -o <output file> -N <number of nearest> -R <radius>` 
* Hypercube: Είτε χωρίς command line arguments (`./bin/HC`), όποτε θα καθοδηγηθούμε από το in-program interface για την αρχικοποίηση των αναγκαίων παραμέτρων, είτε με command line arguments ακολουθώντας τη μορφή `./bin/HC -i <input_file> -q <query file> -k <int> -M <int> -probes <int> -o <output file> -N <number of nearest> -R <radius>`
* Clustering: Με command line arguments ακολουθώντας τη μορφή `./bin/CLUST -i <input file> -c <configuration file> -o <output file> -complete <optional> -m <method: Classic OR LSH or Hypercube>`

## Παραδείγματα εκτέλεσης (για το small_input και small_query datasets)
* LSH
τα default k, L ήταν εντάξει για τα μικρότερα datasets. Ακόμα καλύτερη απόδοση αν αυξήσουμε όμως το k στο 5
./bin/LSH -i ./datasets/input_small_id -q ./datasets/query_small_id -k 5 -L 5 -o LSH_output.txt -N 5 -R 400
τα default k, L ήταν εντάξει για τα μικρότερα datasets 

* Hypercube
Καλές παράμετροι στις δοκιμές μας αποδείχθηκαν οι εξής:
./bin/HC -i ./datasets/input_small_id -q ./datasets/query_small_id -k 7 -M 1000 -probes 10 -o HC_output.txt -N 5 -R 400

Αυξάνοντας το Μ κερδίζουμε σε ακρίβεια και χάνουμε σε χρόνο
./bin/HC -i ./datasets/input_small_id -q ./datasets/query_small_id -k 7 -M 2000 -probes 10 -o HC_output.txt -N 5 -R 400

* Clustering
./bin/CLUST -i ./datasets/input_small_id -c ./cluster.conf -o Clustering_output.txt -m Classic
./bin/CLUST -i ./datasets/input_small_id -c ./cluster.conf -o Clustering_output.txt -m LSH
./bin/CLUST -i ./datasets/input_small_id -c ./cluster.conf -o Clustering_output.txt -m Hypercube

## Ειδικές παρατηρήσεις σχετικά με την υλοποίηση μας
* Το windowSize το υπολογίζουμε δυναμικά ως εξής: 
For dataset.size()/4 samples we randomly choose two points of the dataset and calculate their Euclidean distance.
We sum these disances and calculate the average. Then we multiply that average distance by a factor of our choice and we 
get the windowsize. That factor can be adjusted at LSH.cpp at the initialization of the LSH instance
Απο τις main δίνεται συνήθως το 3 ως παράγοντας για το windowSize. 

Όσο αυξάνεται το windowSize τόσο αυξάνεται η ακρίβεια στους kNN αλλά σε βάρος υπολογιστικού φόρτου και άρα χρόνου.

* Πέρα απο τις δομές της κλάσης Clustering που κρατάνε ποια items είναι assigned σε κάθε cluster, κάθε item έχει ένα πεδίο int
στο οποίο αποθηκεύεται ο index (στον vector των Clusters) του Cluster που είναι currently assigned.
* Οι αλγόριθμοι clustering με reverse assignment τερματίζουν όταν περάσουν τα max iterations που τους προσδιορίζουμε στη main (CLUST.cpp)
* τα items περιέχουν bool flags που αναπαριστούν τη κατάστασή τους σε κάθε σημείο του clustering
marked-> εχει ανατεθεί σε cluster στο ίδιο βήμα, άρα δεν το ξαναελέγχουμε
claimed-> έχει βρεθεί σε άλλη μπάλα κατα το range search του reverse assignment και υπάρχει conflict
* το build των output files γίνεται στις main.