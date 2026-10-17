#include <iostream>
#include <cstring>
#include <typeinfo>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include <list>
#include <stdexcept>
using namespace std;

// format float with 2 decimal places
string format2(float valoare) {
    string text = std::to_string(valoare);
    // find decimal point and keep 2 digits
    return text.substr(0, text.find('.') + 3); 
}

// custom exception hierarchy
class RestaurantException : public std::exception {
    protected:
        string mesaj;
    public:
        RestaurantException(const string& msg) : mesaj(msg) {}

        virtual ~RestaurantException() noexcept{};
        
        // what() with noexcept override
        const char* what() const noexcept override {
            return mesaj.c_str(); // message overridden by derived classes 
        }
};

// table exception
class CapacitateMasaException : public RestaurantException {
    private:
        int capacitateMaxima;
        int cerute;
    public:
        CapacitateMasaException(int max, int cerute)
            : RestaurantException("Eroare Masa: Capacitate depasita!"), capacitateMaxima(max), cerute(cerute) {
            mesaj += " Masa are " + to_string(max) + " locuri, dar s-au cerut " + to_string(cerute) + ".";
        }

        ~CapacitateMasaException() noexcept override {};
};

// product exception
class ProdusInexistentException : public RestaurantException {
    private:
        string numeProdus;
    public:
        ProdusInexistentException(const string& nume)
            : RestaurantException("Eroare Meniu: Produsul nu exista!"), numeProdus(nume) {
            mesaj += " Produsul '" + nume + "' nu a fost gasit in meniu.";
        }

        ~ProdusInexistentException() noexcept override{};
};

// invalid value exception 
class DateInvalideException : public RestaurantException {
    private:
        float valoareGresita;
    public:
        DateInvalideException(float valoare)
            : RestaurantException("Eroare Date: Valoare invalida!"), valoareGresita(valoare) {
            mesaj += " Valoarea " + to_string(valoare) + " nu este permisa (nu poate fi negativa).";
        }

        ~DateInvalideException() noexcept override{};
};

// person not found exception 
class PersoanaNegasitaException : public RestaurantException {
    private:
        string numeCautat;
    public:
        PersoanaNegasitaException(const string& nume)
            : RestaurantException("Eroare Sistem: Persoana nu a fost gasita!"), numeCautat(nume) {
            mesaj += " Nu exista nicio inregistrare pentru numele: '" + nume + "'.";
        }

        ~PersoanaNegasitaException() noexcept override{}
};

// feedback template class (ratings or reviews)
template <typename T, int LimitaMemorie>
class RegistruFeedback {

    private:

        vector<T> istoric; // vector of ratings or reviews

    public:

        // constructor 
        RegistruFeedback(){};

        // add feedback and manage memory bounds 
        void adaugaFeedback(const T& feedback) {
            if(istoric.size() >= LimitaMemorie){
                throw overflow_error("Registrul a atins limita maxima de memorie (" + to_string(LimitaMemorie) + ")!");
            }
            istoric.push_back(feedback);
        }

        // getter for feedback history 
        const vector<T>& getIstoric() const {
            return istoric;
        }

        // template method inside template class
        template <typename U>
        void adaugaFeedbackConvertit(const U& valoare, T (*functieConversie)(U)) { 
            T valoareConvertita = functieConversie(valoare);
            adaugaFeedback(valoareConvertita);
        }
};

// print total entries in register
template <typename T, int N>
void afiseazaStatusRegistru(const RegistruFeedback<T, N>& registru) {
    cout<<"Registrul curent contine "<<registru.getIstoric().size()<<" intrari (din maxim "<<N<<")."<<endl;
}

// generic template
template <typename T>
void genereazaRaport(const vector<T>& date) {
    cout<<"Raport generic: "<<date.size()<<" elemente inregistrate."<<endl;
}

// template specialization for ratings - calculates average
template <>
inline void genereazaRaport<int>(const vector<int>& date) {
    if(date.empty()) {
        cout<<"Nicio nota inregistrata inca."<<endl;
        return;
    }
    float suma = 0;
    for(int nota : date){
        suma += nota;
    }
    cout<<"Restaurantul are "<<(suma / date.size()) 
        <<" stele (din " << date.size()<<" recenzii)."<<endl;
}

// template specialization for reviews - generates text report
template <>
inline void genereazaRaport<std::string>(const vector<std::string>& date) {
    if(date.empty()) {
        cout<<"Nu exista feedback de tip text inregistrat."<<endl;
        return;
    }
    cout<<"RAPORT RESTAURANT (Sugestii si Reclamatii):"<<endl;
    for(int i = 0; i < date.size(); i++) {
        cout<<i + 1<<". " <<date[i]<<endl;
    }
}


class IObject{
    protected:
        static int generatorId;
        const int id;
    public:

        // constructor 
        IObject(): id(++generatorId){
            cout<<"Iobject - constructor apelat"<<endl;
        }

        // virtual destructor for base class
        virtual ~IObject() noexcept{}

        // getter for id
        int getId() const {
            return this->id;
        }

        // pure virtual method
        virtual std::string toString() const = 0;

        // equality operator overload
        bool operator==(const IObject& alt ) const noexcept{
            return id == alt.id;
        }

        // output operator, calls toString polymorphically
        friend ostream& operator<<(ostream& os, const IObject& obj){
            return os<<obj.toString(); // calls overridden toString()
        }
};
int IObject::generatorId = 0;


class Logger {
private:
    vector<string> istoricActiuni; 

    // private constructor
    Logger(){
        istoricActiuni.push_back("[Sistem] Logger-ul a fost initializat.");
    }

public:
    // Meyers singleton instance 
    static Logger& getInstance() {
        static Logger instanta;
        return instanta;
    }

    // disable copy and assignment
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // add log entry
    void log(const string& actiune) {
        istoricActiuni.push_back(actiune);
    }

    // print logs 
    void afiseazaJurnal() const {
        cout<<"JURNAL RESTAURANT "<<endl;
        for(const auto& intrare : istoricActiuni) {
            cout<<intrare<<endl;
        }
    }
};


class DataNastere{
    private:
        int zi;
        int luna;
        int an;
    
    public:
        // default constructor 
        DataNastere(): zi(1), luna(1), an(1920){
            cout<<"DataNastere - Constructor apelat"<<endl;
        }

        // constructor with all parameters
        DataNastere(int zi, int luna, int an): zi(zi), luna(luna), an(an){
            cout<<"DataNastere - Constructor apelat"<<endl;
        }

        // copy constructor
        DataNastere(const DataNastere& sursa): zi(sursa.zi), luna(sursa.luna), an(sursa.an){
            cout<<"DataNastere - Constructor de copiere apelat"<<endl;
        }

        // destructor
        ~DataNastere() noexcept{
            cout<<"DataNastere - Destructor apelat"<<endl;
        }

        // getter for day
        int getZi() const{
            return this->zi;
        }

        // setter for day
        void setZi(int zi_nou){
            if(this->luna == 2 && zi_nou > 28){
                throw DateInvalideException(zi_nou);
            }
            if(zi_nou >= 1 && zi_nou <= 31)
                this->zi = zi_nou;
        }

        // getter for month
        int getLuna() const{
            return this->luna;
        }

        // setter for month
        void setLuna(int luna_nou){
            if(this->zi > 28 && luna_nou == 2){
                throw DateInvalideException(luna_nou);
            }
            if(luna_nou >= 1 && luna_nou <= 12)
                this->luna = luna_nou;
        }

        // getter for year
        int getAn() const{
            return this->an;
        }

        // setter for year
        void setAn(int an_nou){
            if(an_nou > 1900 && an_nou <= 2025){
                this->an = an_nou;
            }
            else{
                throw DateInvalideException(an_nou);
            }
            
        }

        // assignment operator 
        DataNastere& operator=(const DataNastere& sursa){
            if(this != &sursa){
                if(sursa.luna == 2 && sursa.zi > 28){
                    throw DateInvalideException(sursa.zi);
                }

                if (sursa.zi >= 1 && sursa.zi <= 31){
                    this->zi = sursa.zi;
                }

                if(sursa.luna >= 1 && sursa.luna <=12 ){
                    this->luna = sursa.luna;
                }

                if(sursa.an >= 1900 && sursa.an <= 2025){
                    this->an = sursa.an;
                }
            }
            
            return *this;
    
        }

        // friend output operator
        friend ostream& operator<<(ostream& out, const DataNastere& data);

};

// output operator 
ostream& operator<<(ostream& out, const DataNastere& data){
    out<<"Ziua: "<<data.zi<<", "<<" luna: "<<data.luna<<", "<<" anul: "<<data.an<<endl;
    return out;
}


    // map chosen for person catalog: lookup, insert and delete by id in O(log N)

class Persoana : public IObject{

    private:// private fields specific to Persoana management
        static std::map<int, Persoana*> personal; 
        

    protected:// protected fields for derived classes
        std:: string nume;
        char gen;
        long long cnp;
        DataNastere dataNastere; // composition

    public:
    
        // constructor with all parameters 
        Persoana(const std::string& nume, const char gen, long long cnp, int zi, int luna, int an)
            :nume(nume), gen(gen), cnp(cnp), dataNastere(zi,luna,an) {
            cout<<"Persoana - constructor apelat"<<endl;

            if(this->nume.empty() || this->nume.size() <= 1){
                this->nume = "N/A";
            }
        }

        // constructor without birth date
        Persoana(const std::string& nume, const char gen, long long cnp)
            :nume(nume), gen(gen), cnp(cnp),dataNastere(1,1,2000) {
            cout<<"Persoana - constructor apelat"<<endl;

            if(this->nume.empty() || this->nume.size() <= 1){
                this->nume = "N/A";
            }
        }

        // constructor with name and gender
        Persoana(const std::string& nume, const char gen)
            :nume(nume), gen(gen), cnp(0),dataNastere(1,1,2000) {
            cout<<"Persoana - constructor apelat"<<endl;

            if(this->nume.empty() || this->nume.size() <= 1){
                this->nume = "N/A";
            }
        }

        // copy constructor
        Persoana(const Persoana& sursa)
            :nume(sursa.nume), gen(sursa.gen), cnp(sursa.cnp), dataNastere(sursa.dataNastere){
            cout<<"Persoana - constructor de copiere apelat"<<endl;

        }

        // virtual destructor - O(log N) deletion
        virtual ~Persoana() noexcept{
            cout<<"Persoana - destructor apelat"<<endl;

        }


        // getter for name
        std::string getNume() const{
            return this->nume;
        }

        // setter for name
        void setNume(const std::string& nume_nou){
            if(!nume_nou.empty() && nume_nou.size() > 1){
                this->nume = nume_nou;
            }
        }

        // getter for gender
        char getGen() const{
            return this->gen;
        }

        // setter for gender
        void setGen(const char gen_nou){
            if(gen_nou == 'M' || gen_nou == 'F' || gen_nou == 'N'){
                this->gen = gen_nou;
            }
        }

        // getter for cnp
        long long getCnp() const{
            return this->cnp;
        }

        // setter for cnp
        void setCnp(long long cnp_nou){
            if(cnp_nou > 0)
                this->cnp = cnp_nou;
        }

        // getter for birth date
        const DataNastere& getDataNastere() const{
            return this->dataNastere;
        }

        // setter for birth date
        void setDataNastere(int zi_nou, int luna_nou, int an_nou){
            this->dataNastere.setZi(zi_nou);
            this->dataNastere.setLuna(luna_nou);
            this->dataNastere.setAn(an_nou);
        }

        // getter for total persons count
        static int getNrPersoane() {
            return personal.size();
        }

        // getter for staff catalog
        static const std::map<int, Persoana*>& getPersonal(){
            return personal;
        }

         // print staff catalog 
        static void afisarePersonal();

        // add person to staff catalog
        void adaugaInPersonal() {
            personal[this->getId()] = this; // overwrites if id exists
        }

        // remove person from staff catalog
        static void eliminaPersoana(const std::string& numeCautat);
          

        // clear staff catalog
        static void curataPersonal(){
            for (auto const& [id, p] : personal) {
                delete p; 
            }
            personal.clear();
        }

        // calculate salary based on role bonuses
        virtual float calculeazaSalariu() const = 0;

        // display role and role-specific fields 
        virtual std::string afisareRol() const = 0;

        // modifies role-specific fields using dynamic_cast
        static void modificaDate(); // implemented after person hierarchy declarations

        // toString method
        std::string toString() const override{
            std::ostringstream out;
            out<<"ID Persoana: "<<std::to_string(this->getId())<<endl
                <<"Nume: "<<this->nume<<endl
                <<"Gen: "<<this->gen<<endl
                <<"CNP: "<<std::to_string(this->cnp)<<endl
                <<this->dataNastere<<endl;
            return out.str();
        }

        // assignment operator
        Persoana& operator=(const Persoana& sursa){
            if( this != &sursa){ // self-assignment check

                this->nume = sursa.nume;
                this->gen = sursa.gen;
                this->cnp = sursa.cnp;
                this->dataNastere = sursa.dataNastere;
            }

            return *this;
        }
        // abstract class, no input operator 

};
std::map<int, Persoana*> Persoana::personal;



class Angajat : public Persoana{

    protected:// protected fields for derived classes
        float salariu;
        int nrBonusuri;
        float* istoricBonusuri;

        static std::map<int, Angajat*> catalogAngajati;
    
    public:

        // default constructor
        Angajat()
             : Persoana("N/A", 'N'),salariu(0), nrBonusuri(0) {

            cout<<"Angajat - constructor apelat"<<endl;
            this->istoricBonusuri = nullptr;

        }
        // constructor with all parameters
        Angajat(const std::string& nume, char gen, long long cnp, int zi, int luna, int an, float salariu)
            : Persoana(nume,gen,cnp,zi,luna,an), salariu(salariu), nrBonusuri(0){

            cout<<"Angajat - constructor apelat"<<endl;
            this->istoricBonusuri = nullptr;

        }
        
        // constructor without birth date
        Angajat(const std::string& nume, char gen, long long cnp, float salariu )
            : Persoana(nume,gen,cnp), salariu(salariu), nrBonusuri(0){

            cout<<"Angajat - constructor apelat"<<endl;
            this->istoricBonusuri = nullptr;

        }

        // constructor with name and gender 
        Angajat(const std::string& nume, char gen)
            : Persoana(nume,gen), salariu(0), nrBonusuri(0){

            cout<<"Angajat - constructor apelat"<<endl;
            this->istoricBonusuri = nullptr;
        }

        // constructor with name, gender and salary
        Angajat(const std::string& nume, char gen, float salariu)
            : Persoana(nume,gen), salariu(salariu), nrBonusuri(0){

            cout<<"Angajat - constructor apelat"<<endl;
            this->istoricBonusuri = nullptr;
        }

        // copy constructor 
        Angajat(const Angajat& sursa)
            : Persoana(sursa), salariu(sursa.salariu), nrBonusuri(sursa.nrBonusuri){

            cout<<"Angajat - constructor de copiere apelat"<<endl;
            if(sursa.istoricBonusuri != nullptr && sursa.nrBonusuri > 0){
                this->istoricBonusuri = new float[sursa.nrBonusuri];
                for(int i = 0; i < sursa.nrBonusuri; i++){
                    this->istoricBonusuri[i] = sursa.istoricBonusuri[i];
                }
            }
            else{
                this->istoricBonusuri = nullptr;
            }
        }

        // destructor
        virtual ~Angajat() noexcept{

            cout<<"Angajat - destructor apelat"<<endl;
            
            if(this->istoricBonusuri != nullptr){
                delete [] this->istoricBonusuri;
                this->istoricBonusuri = nullptr;
            }
        }


        // getter for salary
        float getSalariu() const{
            return this->salariu;
        }

        // setter for salary
        void setSalariu(float salariu_nou){
            if(salariu_nou > 0)
                this->salariu = salariu_nou;
        }

        // getter for static count
        static int getNrAngajati(){
            return catalogAngajati.size();
        }

        // getter for bonus count
        int getNrBonusuri() const{
            return this->nrBonusuri;
        }

        // bonus count is read-only

        // getter for bonus history
        void afiseazaIstoricBonusuri(){
            if(this->istoricBonusuri != nullptr && this->nrBonusuri > 0){
                cout<<"Istoric bonusuri pentru: "<<this->getNume()<<" :"<<endl;
                float total = 0;
                for(int i = 0; i < this->nrBonusuri; i++){
                    cout<<"  "<<i+1<<". "<<this->istoricBonusuri[i]<<" RON"<<endl;
                    total += this->istoricBonusuri[i];
                }
                cout<<"Total bonusuri acordate: "<<total<<" RON."<<endl;
            }
            else{
                throw RestaurantException("Eroare : nu exista bonusuri acordate!"); 
            }
        }
        
        // display bonus history by name
        static void afiseazaIstoricBonusuriNume(const std::string& numeCautat){
            if(numeCautat.empty() || catalogAngajati.empty()){
                throw RestaurantException("Eroare Catalog: Date invalide sau catalogul de angajati este gol!"); 
            }
            bool gasit = false;
            for(auto const& [id, a] : catalogAngajati){
                if(a->getNume() == numeCautat){
                    a->afiseazaIstoricBonusuri();
                    gasit = true;
                    break;
                }
            }
            if(!gasit)
                throw PersoanaNegasitaException(numeCautat);
        }

        // interactive creation for menu
        static void create(){
            std::string buffer;
            long long cnp_nou;
            float sal_nou;
            char gen_nou;
            int zi_noua;
            int luna_noua;
            int an_nou;

            cout<<"Inregistrare angajat nou"<<endl;
            cout<<"Nume: "; cin>>ws; std::getline(cin, buffer);
            cout<<"Gen(M/F/N): "; cin>>gen_nou;
            cout<<"CNP: "; cin>>cnp_nou;
            cout<<"Data nasterii: "<<endl;
            cout<<"Ziua: "<<endl; cin>>zi_noua;
            cout<<"Luna: "<<endl; cin>>luna_noua;
            cout<<"Anul: "<<endl; cin>>an_nou;
            cout<<"Salariu: "; cin>>sal_nou;

            Angajat* nou = new Angajat(buffer, gen_nou, cnp_nou, zi_noua, luna_noua, an_nou, sal_nou );// create new object 
            nou->adaugaInCatalog(); // add dynamic object to catalog
            nou->adaugaInPersonal();// add to staff catalog
            cout<<"Angajat inregistrat cu succes!"<<endl;
            Logger::getInstance().log("S-a adaugat angajatul " + buffer + " cu ID " + 
                                  to_string(nou->getId()) + " in catalogul de angajati si de personal.");
        }

        // add employee to catalog
        void adaugaInCatalog() {
            catalogAngajati[this->getId()] = this;
        }


        // remove from private map
        static void stergeDinCatalogSpecific(int idDeSters) {
            auto it = catalogAngajati.find(idDeSters);
            if (it != catalogAngajati.end()) {
                catalogAngajati.erase(it);
            }
        }

        // remove employee from catalog 
        static void eliminaAngajat(const std::string& numeCautat){
            if(numeCautat.empty() || catalogAngajati.empty())
                return;

            bool esteAngajat = false;

            for (auto const& [id, a] : catalogAngajati){
                if(a->getNume() == numeCautat){
                    esteAngajat = true;
                    break;
                }
            }
            if(esteAngajat){
                Persoana::eliminaPersoana(numeCautat);// calls base function to delete and remove from personal
            }
            else{
                throw PersoanaNegasitaException(numeCautat);
            }
        }

        // clear catalog
        static void curataCatalog(){
            // clear map; Persoana::curataPersonal handles deletion
            catalogAngajati.clear();
        }

        // grant bonus
        void acordaBonus(float suma){
            if(suma > 0){
                this->salariu += suma;

                float* temp = new float[this->nrBonusuri + 1];
                if(this->istoricBonusuri != nullptr && this->nrBonusuri > 0){
                    for( int i = 0; i< this->nrBonusuri; i++){
                        temp[i] = this->istoricBonusuri[i];
                    }
                    delete[] this->istoricBonusuri;
                }
                temp[this->nrBonusuri] = suma;
                this->istoricBonusuri = temp;
                this->nrBonusuri ++;
            }
        }

        // grant bonus by name
        static void acordaBonusNume(const std::string& numeCautat, float suma){
            if(numeCautat.empty() || catalogAngajati.empty()){
                throw RestaurantException("Eroare Catalog: Date invalide sau catalogul de angajati este gol!");
            }

            bool gasit = false;
            for(auto const& [id, a] : catalogAngajati){
                if(a->getNume() == numeCautat){
                    a->acordaBonus(suma);
                    cout<<"Bonus de "<<suma<<" LEI, acordat lui "<<numeCautat<<endl;
                    gasit = true;
                    break;
                }
            }

            if(!gasit)
                throw PersoanaNegasitaException(numeCautat);
                
        }


        // employee salary statistics and average
        static void afisareStatisticiCatalog() {
            if(catalogAngajati.empty()) {
                throw RestaurantException("Eroare Catalog: catalogul de angajati este gol!");
            }

            float suma = 0;
            // temporary vector of pointers
            std::vector<Angajat*> clasament;
            
            for(auto const& [id, a] : catalogAngajati) {
                clasament.push_back(a);
                suma += a->calculeazaSalariu();
            }

            // sort using lambda
            std::sort(clasament.begin(), clasament.end(), [](Angajat* a1, Angajat* a2) {
                return a1->calculeazaSalariu() > a2->calculeazaSalariu(); 
            });

            cout<<"---- STATISTICI SI CLASAMENT SALARII ----"<<endl;
            cout<<"Media salariala in firma: "<<(suma / catalogAngajati.size())<<" RON" <<endl;
            cout<<"--------------------------------------------------"<<endl;

            int i = 1;
            
            for(Angajat* a : clasament) {
                cout<<i++<< ". " <<a->getNume()<<endl;
                cout<<"Salariu de baza: "<<a->getSalariu()<<" RON"<<endl;
                cout<<"Salariu total: "<<a->calculeazaSalariu()<<" RON" <<endl;
                cout<<endl;
            }
            cout<<endl;
        }

        // display role override 
        std::string afisareRol() const override{
            return "Angajat";  
            
        }

        // calculate salary implementation 
        float calculeazaSalariu() const override {
            return this->salariu;
        }


        // overridden toString method 
        std::string toString() const override {
            std::ostringstream out;
            out<<Persoana::toString() 
                <<"Rol: "<<this->afisareRol()<<endl
                <<"Salariu de baza: "<<format2(this->salariu) << " RON"<<endl
                <<"Bonusuri (" << std::to_string(this->nrBonusuri) << "): "<<endl;

            if(this->istoricBonusuri != nullptr && this->nrBonusuri > 0){
                for(int i = 0; i < this->nrBonusuri; i++){
                    out << format2(this->istoricBonusuri[i]) << " ";
                }
                out<<endl;
            } else {
                out << "0"<<endl;
            }
            out <<"Salariu total calculat: " <<format2(this->calculeazaSalariu()) << " RON";
            
            return out.str();
        }

        // assignment operator
        Angajat& operator=(const Angajat& sursa){
            if( this != &sursa){ // self-assignment check

                Persoana::operator=(sursa);
                this->salariu = sursa.salariu;
    

                this->nrBonusuri = sursa.nrBonusuri;

                if(this->istoricBonusuri != nullptr ){
                    delete[] this->istoricBonusuri;
                    this->istoricBonusuri = nullptr;
                }

                if(sursa.istoricBonusuri != nullptr && sursa.nrBonusuri > 0){
                    this->istoricBonusuri = new float[sursa.nrBonusuri];
                    for(int i = 0; i < sursa.nrBonusuri; i++){
                        this->istoricBonusuri[i] = sursa.istoricBonusuri[i];
                    }
                }
            }

            return *this;
        }

        // bool conversion operator
        explicit operator bool() const{
                return this->salariu > 3500;
            }


        // input operator declaration
        friend istream& operator>>(istream& in, Angajat& a);

        // arithmetic operators
        friend Angajat operator+(const Angajat& a, int cantitate);
        friend Angajat operator+(int cantitate, const Angajat& a);
        friend Angajat operator-(const Angajat& p, int cantitate);
        
        // comparison operators
        friend bool operator<(const Angajat& a1,const Angajat& a2);
        friend bool operator==(const Angajat& a1,const Angajat& a2);

        // increment operators
        Angajat& operator++(); // prefix increment
        Angajat operator++(int); // postfix increment
        Angajat& operator--(); // prefix decrement
        Angajat operator--(int); // postfix decrement

        // subscript operator
        char& operator[](int index);

};

// input operator
istream& operator>>(istream& in, Angajat& a){
    cout<<"Nume angajat: ";
    std::string buffer;
    in>>ws;
    getline(in, buffer);
    a.setNume(buffer);

    long long cnp_nou;
    cout<<"CNP: "; in>>cnp_nou;
    a.setCnp(cnp_nou);
    char gen_nou;
    cout<<"Gen(M/F/N): "; in>>gen_nou;
    a.setGen(gen_nou);
    float salariu_nou;
    int zi_noua, luna_noua, an_nou;
    cout<<"Data nasterii: "<<endl;
    cout<<"Ziua: "<<endl; 
    in>>zi_noua;
    cout<<"Luna: "<<endl;
    in>>luna_noua;
    cout<<"Anul: "<<endl; 
    in>>an_nou;
    a.setDataNastere(zi_noua,luna_noua,an_nou);
    cout<<"Salariu: "; 
    in>>salariu_nou;
    a.setSalariu(salariu_nou);

    return in;
}

// addition operator
Angajat operator+(const Angajat& a, int cantitate){
    Angajat copie = a;
    if(cantitate > 0)
        copie.salariu += cantitate;
    
    return copie;
}

// commutative addition
Angajat operator+(int cantitate, const Angajat& a){
    return a + cantitate;
}

// subtraction operator 
Angajat operator-(const Angajat& a, int cantitate){
    Angajat copie = a;
    if(copie.salariu >= cantitate)
        copie.salariu -= cantitate;

    return copie;

}

// salary comparison
bool operator<(const Angajat& a, const Angajat& b){
    return a.salariu < b.salariu;
}

// salary equality
bool operator==(const Angajat& a, const Angajat& b){
    return a.salariu == b.salariu;
}

// prefix increment
Angajat& Angajat::operator++(){
    this->salariu += 100;
    return *this;
}

// postfix increment
Angajat Angajat::operator++(int){
    Angajat copie = *this;
    this->salariu += 100;
    return copie;
}

// prefix decrement
Angajat& Angajat::operator--(){
    if(this->salariu >= 100)
        this->salariu -= 100;
    return *this;
}

// postfix decrement
Angajat Angajat::operator--(int){
    Angajat copie = *this;
    if(this->salariu >= 100)
        this->salariu -= 100;
    return copie;
}

// subscript operator
char& Angajat::operator[](int index) {
    if (!this->nume.empty() &&  index >= 0 && index < this->nume.size()) {
        return this->nume[index];
    }
    static char eroare = '\0';
    return eroare;
}

std::map<int, Angajat*> Angajat::catalogAngajati;

// print staff catalog
void Persoana::afisarePersonal(){
    cout<<" Personal Restaurant:  ( "<<personal.size()<<" ) persoane"<<endl;

    int index = 1;
    for(auto const& [id, p] : personal){
        if(p != nullptr){

            cout<<index++<<". "<<endl;
            // typeid provides type info name
            cout<<"[Sistem: Obiect identificat in memorie ca: "
            <<typeid(*p).name() << "]" << endl;

            cout<<endl;
            Angajat* ang = dynamic_cast<Angajat*>(p);
            if(ang != nullptr){
                cout<<*ang<<endl;
            }
            else{
                cout<<*p<<endl; // administrator
            }
        }
        cout<<endl;
    }
}

// remove person from staff catalog
void Persoana::eliminaPersoana(const std::string& numeCautat){
    if(numeCautat.empty() || personal.empty())
        return;

    int idDeSters = -1;
    Persoana* deSters = nullptr;

    // iterate through map
    for( auto const& [id,p] : personal){
        if(p->nume == numeCautat){
            deSters = p;
            idDeSters = id;
            break;
        }
    }

    if(deSters != nullptr){
        Angajat::stergeDinCatalogSpecific(idDeSters);
        delete deSters;
        personal.erase(idDeSters);
        cout<<"Persoana: "<<numeCautat<<" a fost stearsa din personal"<<endl;
    }
    else{
        throw PersoanaNegasitaException(numeCautat);
    }
}


class Bucatar: public Angajat{
    private:
        std::string specialitate;
        int aniExperienta;

    public:

        // default constructor
        Bucatar() : Angajat(), specialitate("N/A"),aniExperienta(0){

            cout<<"Bucatar - constructor apelat"<<endl;
        }

        // constructor with all parameters
        Bucatar(const std::string& nume, const char gen, long long cnp, int zi, int luna, int an, float salariu,
             const std::string& specialitate, int aniExperienta )
            : Angajat(nume, gen, cnp, zi, luna, an, salariu), specialitate(specialitate), aniExperienta(aniExperienta){

            cout<<"Bucatar - constructor apelat"<<endl;
            if(this->specialitate.empty()){
                this->specialitate = "N/A";
            }
        }

        // constructor without birth date
        Bucatar(const std::string& nume, const char gen, long long cnp, float salariu,
             const std::string& specialitate, int aniExperienta )
            : Angajat(nume, gen, cnp, salariu),specialitate(specialitate), aniExperienta(aniExperienta){

            cout<<"Bucatar - constructor apelat"<<endl;
            if(this->specialitate.empty()){
                this->specialitate = "N/A";
            }
        }

        // constructor without birth date and cnp
        Bucatar(const std::string& nume, const char gen,float salariu,
             const std::string& specialitate, int aniExperienta )
            : Angajat(nume, gen, salariu),specialitate(specialitate), aniExperienta(aniExperienta){

            cout<<"Bucatar - constructor apelat"<<endl;
            if(this->specialitate.empty()){
                this->specialitate = "N/A";
            }
        }

        // constructor without birth date, cnp and experience
        Bucatar(const std::string& nume, const char gen,float salariu,
             const std::string& specialitate )
            : Angajat(nume, gen, salariu),aniExperienta(0) {

            cout<<"Bucatar - constructor apelat"<<endl;
            if(this->specialitate.empty()){
                this->specialitate = "N/A";
            }
        }

        // constructor with name, gender and salary 
        Bucatar(const std::string& nume, const char gen,float salariu)
            : Angajat(nume, gen, salariu),aniExperienta(0) {

            cout<<"Bucatar - constructor apelat"<<endl;
            this->specialitate = "N/A";
            
    
        }

        // copy constructor
        Bucatar(const Bucatar& sursa)
            :Angajat(sursa),specialitate(sursa.specialitate), aniExperienta(sursa.aniExperienta){

            cout<<"Bucatar - constructor de copiere apelat"<<endl;
        }

        // destructor
        virtual ~Bucatar() noexcept{
            cout<<"Bucatar - destructor apelat"<<endl;

        }

        // getter for yeari experienta
        int getAniExperienta() const{
            return this->aniExperienta;
        }

        // setter for yeari experienta
        void setAniExperienta( int ani){
            if(ani >= 0)
                this->aniExperienta = ani;
        }

        // getter for specialty
        std::string getSpecializare() const{
            return this->specialitate;
        }

        // setter for specialty
        void setSpecializare( const std::string& special){
            if(!special.empty()){
                this->specialitate = special;
            }
        }

        // display role
        std::string afisareRol() const override{
            return "Bucatar";

        }

        // calculate salary: 100 bonus per year of experience
        float calculeazaSalariu() const override{
            return this->salariu + (this->aniExperienta * 100.0f);
        }


        // interactive creation
        static void create(){
            std::string buffer;
            long long cnp_nou;
            char gen_nou;
            int zi_noua;
            int luna_noua;
            int an_nou;
            float sal_nou;
            int aniExp_noi;
            std::string spec;

            cout<<"Inregistrare angajat nou"<<endl;
            cout<<"Nume: "; cin>>ws; getline(cin, buffer);
            cout<<"Gen(M/F/N): "; cin>>gen_nou;
            cout<<"CNP: "; cin>>cnp_nou;
            cout<<"Data nasterii: "<<endl;
            cout<<"Ziua: "<<endl; cin>>zi_noua;
            cout<<"Luna: "<<endl; cin>>luna_noua;
            cout<<"Anul: "<<endl; cin>>an_nou;
            cout<<"Salariu de baza: "; cin>>sal_nou;
            cout<<"Specialitate: "; cin>>ws; getline(cin, spec);
            cout<<"Ani experienta: "; cin>>aniExp_noi;
        
            Bucatar* nou = new Bucatar(buffer, gen_nou, cnp_nou, zi_noua, luna_noua, an_nou, sal_nou , spec, aniExp_noi);// create new object 
            nou->adaugaInCatalog(); // add dynamic object to employee catalog
            nou->adaugaInPersonal(); // add to staff catalog
            cout<<"Bucatar inregistrat cu succes!"<<endl;
            Logger::getInstance().log("S-a adaugat bucatarul " + buffer + " cu ID " + 
                                  to_string(nou->getId()) + " in catalogul de angajati si de personal.");
        }

        // overridden toString method 
        std::string toString() const override {
            std::ostringstream out;
            
            // call base class method
            out<<Angajat::toString()<<endl 
                <<"Specializare: " << this->specialitate<<endl
                <<"Ani experienta: " << std::to_string(this->aniExperienta)<<endl;
                
            return out.str();
        }

         // assignment operator
        Bucatar& operator=(const Bucatar& sursa){
            if(this != &sursa){
                Angajat::operator=(sursa);
                this->aniExperienta = sursa.aniExperienta;
                this->specialitate = sursa.specialitate;
            }
            return *this;
        }

};

class Ospatar final : public Angajat{ // final: cannot be inherited
    private:
        int etaj;
        int schimb;

    public:
        // default constructor
        Ospatar(): Angajat(), etaj(0), schimb(0){
            cout<<"Ospatar - constructor apelat"<<endl;
        }

        // constructor with all parameters
        Ospatar(const std::string& nume, const char gen, long long cnp, int zi, int luna, int an,
            float salariu , int etaj, int schimb)
            :Angajat(nume, gen, cnp, zi, luna, an, salariu), etaj(etaj), schimb(schimb){
            cout<<"Ospatar - constructor apelat"<<endl;
        }

        // constructor without birth date
        Ospatar(const std::string& nume, const char gen, long long cnp,
            float salariu , int etaj, int schimb)
            :Angajat(nume, gen, cnp, salariu), etaj(etaj), schimb(schimb){
            cout<<"Ospatar - constructor apelat"<<endl;
        }

        // constructor without birth date and cnp
        Ospatar(const std::string& nume, const char gen, float salariu , int etaj, int schimb)
            :Angajat(nume, gen,salariu), etaj(etaj), schimb(schimb){
            cout<<"Ospatar - constructor apelat"<<endl;
        }

        // constructor with name, gender and salary
        Ospatar(const std::string& nume, const char gen, float salariu )
            :Angajat(nume, gen,salariu), etaj(0), schimb(0){
            cout<<"Ospatar - constructor apelat"<<endl;
        }

        // copy constructor
        Ospatar(const Ospatar& sursa)
            :Angajat(sursa), etaj(sursa.etaj), schimb(sursa.schimb){
            cout<<"Ospatar - constructor de copiere apelat"<<endl;
        }

        // destructor
        virtual ~Ospatar() noexcept{
            cout<<"Ospatar - destructor apelat"<<endl;
        }

        // getter for floor
        int getEtaj() const{
            return this->etaj;
        }

        // setter for floor
        void setEtaj(int etaj_nou){
            if(etaj_nou > 0 && etaj_nou < 4)
                this->etaj = etaj_nou;
        }

        // getter for shift
        int getSchimb() const{
            return this->schimb;
        }

        // setter for shift
        void setSchimb(int schimb_nou){
            if(schimb_nou > 0 && schimb_nou < 3)
                this->schimb = schimb_nou;
        }

        // display role
        std::string afisareRol() const override{
            return "Ospatar";

        }

        // calculate salary: 100 bonus per year of experience
        float calculeazaSalariu() const override{
            return this->salariu + (this->schimb * 100.0f);
        }


        // interactive creation
        static void create(){
            std::string buffer;
            long long cnp_nou;
            char gen_nou;
            int zi_noua;
            int luna_noua;
            int an_nou;
            float sal_nou;
            int etaj;
            int schimb;

            cout<<"Inregistrare angajat nou"<<endl;
            cout<<"Nume: "; cin>>ws; getline(cin, buffer);
            cout<<"Gen(M/F/N): "; cin>>gen_nou;
            cout<<"CNP: "; cin>>cnp_nou;
            cout<<"Data nasterii: "<<endl;
            cout<<"Ziua: "<<endl; cin>>zi_noua;
            cout<<"Luna: "<<endl; cin>>luna_noua;
            cout<<"Anul: "<<endl; cin>>an_nou;
            cout<<"Salariu de baza: "; cin>>sal_nou;
            cout<<"Etaj: "; cin>>etaj;
            cout<<"Schimb: "; cin>>schimb;
        
            Ospatar* nou = new Ospatar(buffer, gen_nou, cnp_nou, zi_noua, luna_noua, an_nou, sal_nou , etaj, schimb);// create new object 
            nou->adaugaInCatalog(); // add dynamic object to employee catalog
            nou->adaugaInPersonal(); // add to staff catalog
            cout<<"Ospatar inregistrat cu succes!"<<endl;
            Logger::getInstance().log("S-a adaugat ospatarul " + buffer + " cu ID " + 
                                  to_string(nou->getId()) + " in catalogul de angajati si de personal.");
        }

        // toString method
        std::string toString() const override{
            std::ostringstream out;

            out<<Angajat::toString()<<endl
                <<"Etaj: "<<std::to_string(this->etaj)<<endl
                <<"Schimb: "<<std::to_string(this->schimb)<<endl;

            return out.str();
        }

         // assignment operator
        Ospatar& operator=(const Ospatar& sursa){
            if(this != &sursa){
                Angajat::operator=(sursa);
                this->etaj = sursa.etaj;
                this->schimb = sursa.schimb;
            }
            return *this;
        }

};


class Administrator : public Persoana{
    private:

        float profit;
        std::string numeRestaurant;
        int aniFunctionare;

    public:

        // default constructor
        Administrator()
             : Persoana("N/A", 'N'), profit(0),numeRestaurant("N/A"), aniFunctionare(0) {

            cout<<"Administrator - constructor apelat"<<endl;

        }

        // constructor with all parameters
        Administrator(const std::string& nume, char gen, long long cnp, int zi, int luna, int an, float profit, const std::string& numeRestaurant, int aniFunctionare)
            : Persoana(nume,gen,cnp,zi,luna,an), profit(profit),numeRestaurant(numeRestaurant), aniFunctionare(aniFunctionare){

            cout<<"Administrator - constructor apelat"<<endl;
            if(this->numeRestaurant.empty()){
                this->numeRestaurant = "N/A";
            }

        }
        
        // constructor without birth date
        Administrator(const std::string& nume, char gen, long long cnp, float profit, const std::string& numeRestaurant, int aniFunctionare)
            : Persoana(nume,gen,cnp), profit(profit),numeRestaurant(numeRestaurant), aniFunctionare(aniFunctionare){

            cout<<"Administrator - constructor apelat"<<endl;
            if(this->numeRestaurant.empty()){
                this->numeRestaurant = "N/A";
            }

        }

        // constructor without birth date and cnp 
        Administrator(const std::string& nume, char gen, float profit, const std::string& numeRestaurant, int aniFunctionare)
            : Persoana(nume,gen), profit(profit),numeRestaurant(numeRestaurant), aniFunctionare(aniFunctionare){

            cout<<"Administrator - constructor apelat"<<endl;
            if(this->numeRestaurant.empty()){
                this->numeRestaurant = "N/A";
            }

        }
        
        // copy constructor 
        Administrator(const Administrator& sursa)
            : Persoana(sursa), profit(sursa.profit),numeRestaurant(sursa.numeRestaurant), aniFunctionare(sursa.aniFunctionare){

            cout<<"Administrator - constructor de copiere apelat"<<endl;
        }

        // destructor
        virtual ~Administrator() noexcept{

            cout<<"Administrator - destructor apelat"<<endl;
        }

        // getter for profit
        float getProfit() const{
            return this->profit;
        }

        // setter for profit
        void setProfit(float profit_nou){
            if(profit_nou > 0)
                this->profit = profit_nou;
        }

        // getter for name restaurant
        std::string getNumeRestaur() const{
            return this->numeRestaurant;
        }

        // setter for name restaurant
        void setNumeRestaurant( const std::string& nume_nou){
            if(!nume_nou.empty()){
                this->numeRestaurant = nume_nou;
            }
        }

        // getter for yeari functionare
        int getAniFunctionare() const{
            return this->aniFunctionare;
        }

        // setter for yeari functionare
        void setAniFunctionare( int ani){
            if(ani > 0){
                this->aniFunctionare = ani;
            }
            else{
                throw DateInvalideException(ani);
            }
        }

        // interactive creation
        static void create(){
            std::string buffer;
            long long cnp_nou;
            char gen_nou;
            int zi_noua;
            int luna_noua;
            int an_nou;
            float profit_nou;
            std::string nume;
            int aniFunc;

            cout<<"Inregistrare angajat nou"<<endl;
            cout<<"Nume: "; cin>>ws; getline(cin,buffer);
            cout<<"Gen(M/F/N): "; cin>>gen_nou;
            cout<<"CNP: "; cin>>cnp_nou;
            cout<<"Data nasterii: "<<endl;
            cout<<"Ziua: "<<endl; cin>>zi_noua;
            cout<<"Luna: "<<endl; cin>>luna_noua;
            cout<<"Anul: "<<endl; cin>>an_nou;
            cout<<"Profit: "; cin>>profit_nou;
            cout<<"Nume restaurant: "; cin>>ws; getline(cin,nume);
            cout<<"Ani de functionare: "; cin>>aniFunc;
        
            Administrator* nou = new Administrator(buffer, gen_nou, cnp_nou, zi_noua, luna_noua, an_nou, profit_nou, nume, aniFunc);// create new object 
            nou->adaugaInPersonal(); // add to staff catalog
            cout<<"Administrator inregistrat cu succes!"<<endl;
            Logger::getInstance().log("S-a adaugat administratorul " + buffer + " cu ID " + 
                                  to_string(nou->getId()) + " in catalogul personalului.");
        }

         // display role
        std::string afisareRol() const override{
            return "Administrator";
        }

        // calculate salary based on profit
        float calculeazaSalariu() const override{
            return this->profit - (this->profit * 0.2);
        }

        // toString method
        std::string toString() const override {
            std::ostringstream out;
            
            out<<Persoana::toString()<<endl 
                <<"Rol: "<<this->afisareRol()<<endl
                <<"Profit: "<<format2(this->profit)<<" RON"<<endl
                <<"Nume Restaurant: "<< this->numeRestaurant<<endl
                <<"Ani functionare: "<<to_string(this->aniFunctionare)<<endl;
                
            return out.str();
        }

         // assignment operator
        Administrator& operator=(const Administrator& sursa){
            if( this != &sursa){ // self-assignment check

                Persoana::operator=(sursa);
                this->profit = sursa.profit;
                this->aniFunctionare = sursa.aniFunctionare;
                this->numeRestaurant = sursa.numeRestaurant;
            }

            return *this;
        }

};

class Produs : public IObject{

    private:

        std::string nume;
        float pret;
        int calorii;
        int stoc;

        // vector chosen for menu: stable structure with O(1) random access
       static std::vector<Produs*> meniu;

    public:

        // default constructor
        Produs(): nume("N/A") ,pret(0.0), calorii(0), stoc(0){
            cout<<"Produs - constructor apelat"<<endl;
        }
        
        // constructor with all parameters
        Produs(const std::string& nume, float pret, int calorii, int stoc)
            :nume(nume),pret(pret), calorii(calorii), stoc(stoc){
                
            cout<<"Produs - constructor apelat"<<endl;
            if(nume.empty()){
                this->nume = "N/A";
            }

        }

        // constructor with name and price
        Produs(const std::string& nume, float pret )
            :nume(nume), pret(pret), calorii(0), stoc(0){
            
            cout<<"Produs - constructor apelat"<<endl;
            if(nume.empty()){
                this->nume = "N/A";
            }
        }
        
        // constructor with name, price and stock
        Produs(const std::string& nume, float pret,int stoc)
            : pret(pret), calorii(0), stoc(stoc){

            cout<<"Produs - constructor apelat"<<endl;
            if(nume.empty()){
                this->nume = "N/A";
            }

        }

        // copy constructor
        Produs(const Produs& sursa) 
            :nume(sursa.nume), pret(sursa.pret), calorii(sursa.calorii), stoc(sursa.stoc) {

            cout<<"Produs - constructor de copiere apelat"<<endl;
        }

        // destructor
        ~Produs() noexcept{

            cout<<"Produs - destructor apelat"<<endl;
        }

        // getter for price
        float getPret() const{
            return this->pret;
        }

        // setter for price
        void setPret(float pret){
            if(pret >= 1)
                this->pret = pret;
            else 
                throw DateInvalideException(pret);
        }

        // getter for calories
        int getCalorii() const{
            return this->calorii;
        }

        // setter for calories
        void setCalorii(int calorii){
            if(calorii > 0)
                this->calorii = calorii;
        }

        // getter for stock
        int getStoc() const{
            return this->stoc;
        }

        // setter for stock
        void setStoc(int stoc){
            if(stoc > 0)
                this->stoc = stoc;
        }

        //Getter nume
        std::string getNume() const{
            return this->nume;
        }

        //Setter nume
        void setNume(const std::string& nume_nou){
            if(!nume_nou.empty()){
                this->nume = nume_nou;
            }
            else{
                this->nume = "N/A";
            }
        }

        // getter for static count
        static int getNrProduseInMeniu() {
            return meniu.size();
        }

        // interactive creation for menu
        static void create() {
            std::string buffer;
            float p;
            int c, s;

            cout<<"Nume produs: "; cin >> ws; getline(cin, buffer);
            cout<<"Pret: "; cin >> p;
            cout<<"Calorii: "; cin >> c;
            cout<<"Stoc: "; cin >> s;

            Produs* nou = new Produs(buffer, p, c, s);
            nou->adaugaInMeniu(); // add dynamic object to menu
            cout << "Produs adaugat cu succes!"<<endl;
            // logger integration
            Logger::getInstance().log("S-a adaugat produsul '" + buffer + " cu ID " + 
                                  to_string(nou->getId()) + " in meniu.");
        }

        // print menu
        static void afisareMeniu(){
            cout<<" Meniu Restaurant ( "<<meniu.size()<<" ) produse"<<endl;

            int i = 1;
            for(Produs* p : meniu){
                cout<<i++<<". "<<*p<<endl;
            }
        }

        // print menu sorted by price
        static void afiseazaMeniuSortatDupaPret() {
            if (meniu.empty()) { 
                throw RestaurantException("Eroare Meniu: Nu se poate sorta deoarece meniul este gol!");
            }

            auto meniuCopie = meniu; // make copy of menu to preserve order

            std::sort(meniuCopie.begin(), meniuCopie.end(), [](const Produs* a, const Produs* b) {
                return a->getPret() < b->getPret(); // sort ascending by price
            });

            cout<<"MENIU SORTAT DUPĂ PREȚ (CRESCĂTOR) "<<endl;
            int i = 1;
            for (const auto* p : meniuCopie) {
                cout<<i++<<". "<<*p<<endl;
            }
        }

        // add product to menu 
        void adaugaInMeniu() {
            if(std::find(meniu.begin(), meniu.end(), this) == meniu.end()){ // check if already in menu
                meniu.push_back(this);
            }
        }

        // remove product from menu 
        static void eliminaProdus(const std::string& numeCautat){
            if(numeCautat.empty() || meniu.empty())
                return;

            // remove matching items
            auto it = std::remove_if(meniu.begin(), meniu.end(), [&numeCautat](Produs* p){
                if(p->getNume() == numeCautat){
                    delete p;
                    return true;
                }
                return false;
            });
            
            if(it != meniu.end()){
                meniu.erase(it, meniu.end());
                cout<<"Produsul: "<<numeCautat<<" a fost eliminat din meniu."<<endl;
            }
            else{
                throw ProdusInexistentException(numeCautat);
            }  
        }

        // clear menu
        static void curataMeniu(){
           for(Produs* p : meniu){
                delete p;
           }
           meniu.clear();
        }

        // apply discount
        void aplicaReducere(float procent) {
            if (procent > 0 && procent <= 100) {
                this->pret -= this->pret * (procent / 100);
            }
        }

        // apply discount by name
        static void aplicaReducereInMeniu(const std::string& numeCautat, float procent){
            if(numeCautat.empty()){
               throw RestaurantException("Eroare Meniu: Nume invalid sau meniul este gol!");
            }

            bool gasit = false;
            for( Produs* p : meniu){
                if(p->getNume() == numeCautat){
                    p->aplicaReducere(procent);
                    gasit = true;
                    break;
                }
            }

            if(!gasit)
                throw ProdusInexistentException(numeCautat);
        }

        // find product by name
        static Produs* getProdusDinMeniu(const std::string& numeP){
            for(Produs* p : meniu){
                if(p->getNume() == numeP)
                    return p;
            }
            return nullptr;
        }

        // toString method
        std::string toString() const override {
            std::ostringstream out;
            
            out<<"ID Produs: "<<this->id<<endl
                <<"Nume: "<<this->nume<<endl
                <<"Pret: "<<format2(this->pret)<<" RON"<<endl
                <<"Calorii: "<<to_string(this->calorii)<<endl
                <<"Stoc: "<<to_string(this->stoc)<<endl;
                
            return out.str();
        }

        // assignment operator
        Produs& operator=(const Produs& sursa){
            if(this != &sursa){ // self-assignment check

                this->pret = sursa.pret;
                this->calorii = sursa.calorii;
                this->stoc = sursa.stoc;
                this->nume = sursa.nume;
            }

            return *this; // return reference for chaining 
        }
    
        // input operator declaration
        friend istream& operator>>(istream& in, Produs& p);

        // arithmetic operators
        friend Produs operator+(const Produs& p, int cantitate);
        friend Produs operator+(int cantitate, const Produs& p);
        friend Produs operator-(const Produs& p, int cantitate);
        
        // comparison operators
        friend bool operator<(const Produs& p1, const Produs& p2);
        friend bool operator==(const Produs& p1, const Produs& p2);

        // increment operators
        Produs& operator++(); // prefix increment
        Produs operator++(int); // postfix increment
        Produs& operator--(); // prefix decrement
        Produs operator--(int); // postfix decrement

        // subscript operator
        char& operator[](int index);

        // int conversion operator
        explicit operator int() const {
            return this->stoc;
        }

};

// input operator
istream& operator>>(istream& in, Produs& p){
    cout<<"Introduceti numele:";
    // temporary buffer 
    std::string buffer;
    in>>ws;
    getline(in, buffer);
    p.setNume(buffer);
    cout<<"Pret: "; in>>p.pret;
    cout<<"Calorii: "; in>>p.calorii;
    cout<<"Stoc: "; in>>p.stoc;

    return in;
}

// addition operator
Produs operator+(const Produs& p, int cantitate) {
    Produs copie = p;
    if (cantitate > 0) copie.stoc += cantitate;
    return copie;
}

// commutative addition
Produs operator+(int cantitate, const Produs& p) {
    return p + cantitate; 
}

// subtraction operator
Produs operator-(const Produs& p, int cantitate) {
    Produs copie = p;
    if (p.stoc - cantitate >= 0) copie.stoc -= cantitate;
    else copie.stoc = 0;
    return copie;
}

// less-than comparison
bool operator<(const Produs& p1, const Produs& p2) {
    return p1.pret < p2.pret;
}

// equality operator
bool operator==(const Produs& p1, const Produs& p2) {
    return (p1.nume == p2.nume && p1.pret == p2.pret);
}

// prefix increment
Produs& Produs::operator++(){
    this->stoc++;
    return *this;
}

// postfix increment
Produs Produs::operator++(int){
    Produs copie = *this;
    this->stoc++;
    return copie;
}

// prefix decrement
Produs& Produs::operator--(){
    if(this->stoc > 0)
        this->stoc--;
    return *this;
    
}

// postfix decrement
Produs Produs::operator--(int){
    Produs copie = *this;
    if(this->stoc > 0)
        this->stoc--;
    return copie;
    
}

// subscript operator
char& Produs::operator[](int index) {
    if (index >= 0 && index < (int)this->nume.size()) {
        return this->nume[index];
    }
    static char eroare = '\0';
    return eroare;
}

std::vector<Produs*> Produs::meniu;



class Masa : public IObject{
    private:

        int capacitate;
        int nrPersoane;
        bool status;
        std::string observatii;

        // vector chosen for table catalog: fixed size and quick indexed access
        static std::vector<Masa*> catalogMese;

    public:

        // default constructor
        Masa() : capacitate(0), nrPersoane(0), status(false), observatii("N/A"){

            cout<<"Masa - constructor apelat"<<endl;
        }

        // constructor with all parameters
        Masa(int capacitate, int nrPersoane, bool status, const std::string& obs)
            : capacitate(capacitate), nrPersoane(nrPersoane), status(status), observatii(obs){

            cout<<"Masa - constructor apelat"<<endl;
            Logger::getInstance().log("S-a creat masa cu ID " + to_string(this->id) + " (" + to_string(nrPersoane) + " locuri).");
            if(this->observatii.empty()){
                this->observatii = "N/A";
            }

        }

        // constructor with capacity and person count
        Masa(int capacitate, int nrPersoane)
            : capacitate(capacitate), nrPersoane(nrPersoane), observatii("N/A") {

            cout<<"Masa - constructor apelat"<<endl;
            if(nrPersoane > 0 )
                status = true;
            else
                status = false;

        }

        // constructor with person count and status
        Masa(int nrPersoane, bool status)
            :capacitate(0), nrPersoane(nrPersoane), status(status), observatii("N/A"){

            cout<<"Masa - constructor apelat"<<endl;
        }

        // copy constructor
        Masa(const Masa& m)
            :capacitate(m.capacitate), nrPersoane(m.nrPersoane), status(m.status), observatii(m.observatii) {

            cout<<"Masa - constructor de copiere apelat"<<endl;
        }

        ~Masa() noexcept{

            cout<<"Masa - destructor apelat"<<endl;

        }

        // getter for capacity
        int getCapacitate() const{
            return this->capacitate;
        }

        // setter for capacity
        void setCapacitate(int capacitate_noua){
            if(capacitate_noua > 0)
                this->capacitate = capacitate_noua;
        }

        // getter for person count
        int getNrPersoane() const{
            return this->nrPersoane;
        }

        // setter for person count
        void setNrPersoane(int nr_nou){
            if(nr_nou >= 0 && nr_nou <= this->capacitate){
                this->nrPersoane = nr_nou;
            }
        }

        // getter for status
        bool getStatus() const{
            return this->status;
        }

        // setter for status
        void setStatus(bool status_nou){
            this->status = status_nou;
        }

        // getter for notes
        std::string getObs() const{
            return this->observatii;
        }

        // setter for notes
        void setObs(const std::string& obs_noi){
            if(!obs_noi.empty()){
                this->observatii = obs_noi;
            }
        }

         // getter for total tables count
        static int getNrMese(){
            return catalogMese.size();
        }

        // check if table can seat group 
        bool poatePrimiGrup(int nrPers) const{
            return(!this->status && this->capacitate >= nrPers);
        }

        // free table
        void elibereazaMasa(){
            this->nrPersoane = 0;
            this->status = false;
            this->setObs("Masa tocmai a fost eliberata. Trebuie curatata");
            cout<<"Masa a fost eliberata cu succes"<<endl;
        }

        // get table by id 
        static Masa* getMasaDupaId(int idCautat){
            auto it = std::find_if(catalogMese.begin(), catalogMese.end(), [idCautat](Masa* m){
                return m->getId() == idCautat;
            });

            if(it != catalogMese.end()){
                return *it;
            }

            throw RestaurantException("Eroare Masa: Masa cu ID " + to_string(idCautat) + " nu a fost gasita!");
            return nullptr;
        }

        // interactive table creation
        static void createMasa(){
            int cap, nrP;
            std::string buffer;

            cout<<"---Creare masa noua---"<<endl;
            cout<<"Capacitate: "; cin>>cap;
            cout<<"Numar persoane asezate: "; cin>>nrP;
            cout<<"Observatii: "; 
            cin>>ws;
            getline(cin, buffer);

            // add dynamic object to catalog 
            Masa* noua = new Masa(cap, nrP, (nrP > 0), buffer);
            noua->adaugaInCatalog();
            cout<<"Masa a fost adaugata in catalog."<<endl;
            Logger::getInstance().log("S-a creat masa cu ID " + to_string(noua->getId()) + " (" + to_string(cap) + " locuri).");

        }

        // occupy table
        void ocupaMasa(int nrP){
            if(this->status == true){
                throw RestaurantException("Eroare Masa: Masa " + to_string(this->id) + " este deja ocupata!");
            }
            if(nrP > this->capacitate){
               throw CapacitateMasaException(this->capacitate, nrP);
            }

            this->nrPersoane = nrP;
            this->status = true;
            this->observatii = "Masa ocupata recent.";
            cout<<"Masa "<<this->id<<" a fost ocupata cu succes."<<endl;

        }

        // count free tables
        static int nrMeseLibere(){
            if(catalogMese.empty()){
                cout<<"Nu exista mese in catalog."<<endl;
                return 0;
            }

            int meseLibere = std::count_if(catalogMese.begin(), catalogMese.end(), [](Masa* m){
                return m->status == false;
            });

            cout<<"Mese libere: "<<meseLibere<<" din totalul de "<<catalogMese.size()<<" mese."<<endl;

            return meseLibere;
        }

        // find and display free tables
        static void cautaMeseLibere(int nrP){

            int libere = nrMeseLibere();
            if(libere == 0){
                cout<<"Nu avem mese libere"<<endl;
                return;
            }
            
            bool gasit = false;
            for(Masa* m : catalogMese){
                if(m->poatePrimiGrup(nrP)){
                    cout<<*m<<endl;
                    gasit = true;
                }
            }
            if(!gasit){
                throw RestaurantException("Eroare Masa: Nu s-a gasit nicio masa libera pentru " + to_string(nrP) + " persoane.");
            }
        }

        // add table to catalog
        void adaugaInCatalog() {
            if(std::find(catalogMese.begin(), catalogMese.end(), this) == catalogMese.end()){
                catalogMese.push_back(this);
            }
        }

        // remove table from catalog
        static void eliminaMasa(int idCautat){
            auto it = remove_if(catalogMese.begin(), catalogMese.end(), [idCautat](Masa* m){
                if(m->getId() == idCautat){
                    delete m;
                    return true;
                }
                return false;
            });

            // erase removed elements from vector
            if(it != catalogMese.end()){
                catalogMese.erase(it, catalogMese.end());
                cout<<"Masa cu id: "<<idCautat<<" a fost eliminata cu succes"<<endl;
            }
            else{
                throw RestaurantException("Eroare Masa: Nu exista masa cu ID-ul " + to_string(idCautat) + "!");
            }
        }

        // print table catalog
        static void afisareCatalog(){
            cout<<" Stare mese terasa ( "<<catalogMese.size()<<" mese )"<<endl;

            int i = 1;
            for(Masa* m : catalogMese){
                if(m != nullptr)
                    cout<<i++<<". "<<*m<<endl;
            }
        }

        // clear catalog
        static void curataCatalog(){
            for(Masa* m : catalogMese){
                delete m;
            }
            catalogMese.clear();
        }


        // reset table data
        void stergeDateMasa(){
            this->capacitate = 0;
            this->nrPersoane = 0;
            this->status = false;
            this->observatii = "N/A";

            cout<<"Datele mesei au fost sterse"<<endl;
        }

        // toString method
        std::string toString() const override {
            std::ostringstream out;
            std::string obsP = this->observatii;

            // highlight VIP notes
            size_t pos = obsP.find("VIP");
            if (pos != std::string::npos) {
                obsP.replace(pos, 3, "***VIP***"); 
            }
            // truncate long notes 
            if (obsP.length() > 30) {
                obsP = obsP.substr(0, 27); 
                obsP.append("..."); // add ellipsis
            }
            
            out<<"ID Masa: "<<to_string(this->getId())<<endl
                <<"Capacitate: "<<to_string(this->capacitate)<<endl
                <<"Numar persoane: "<<to_string(this->nrPersoane)<<endl
                <<"Status: "<<to_string(this->status)<<endl
                <<"Observatii: "<<obsP<<endl;
                
            return out.str();
        }

        // assignment operator
        Masa& operator=( const Masa& m){
            if(this != &m){
                this->capacitate = m.capacitate;
                this->nrPersoane = m.nrPersoane;
                this->status = m.status;
                this->observatii = m.observatii;

            }

            return *this;
        }

        // bool conversion operator
        explicit operator bool() const{
            return this->status;
        }

        // input operator declaration
        friend istream& operator>>(istream& in, Masa& m);

        // arithmetic operators
        friend Masa operator+(const Masa& m, int cantitate);
        friend Masa operator+(int cantitate, const Masa& m);
        friend Masa operator-(const Masa& m, int cantitate);
        
        // comparison operators
        friend bool operator<(const Masa& m1,const Masa& m2);
        friend bool operator==(const Masa& m1,const Masa& m2);

        // increment operators
        Masa& operator++(); // prefix increment
        Masa operator++(int); // postfix increment
        Masa& operator--(); // prefix decrement
        Masa operator--(int); // postfix decrement

        // subscript operator
        char& operator[](int index);

};


// input operator
istream& operator>>(istream& in, Masa& m){

    cout<<"Capacitate: "; in>>m.capacitate;
    cout<<"Numar persoane asezate: "; in>>m.nrPersoane;
    m.status = (m.nrPersoane > 0);

    cout<<"Observatii: ";
    std::string buffer;
    in>>ws;
    getline(cin, buffer);
    m.observatii = buffer;

    return in;
}

// addition operator
Masa operator+(const Masa& m, int cantitate){
    Masa copie = m;
    if(cantitate > 0)
        copie.capacitate += cantitate;
    
    return copie;
}

// commutative addition
Masa operator+(int cantitate, const Masa& m){
    return m + cantitate;
}

// subtraction operator 
Masa operator-(const Masa& m, int cantitate){
    Masa copie = m;
    if(copie.capacitate >= cantitate)
        copie.capacitate -= cantitate;

    return copie;

}

// capacity comparison
bool operator<(const Masa& a, const Masa& b){
    return a.capacitate < b.capacitate;
}

// capacity equality
bool operator==(const Masa& a, const Masa& b){
    return a.capacitate == b.capacitate;
}

// prefix increment
Masa& Masa::operator++(){
    if(this->nrPersoane < this->capacitate){
        this->nrPersoane++;
        this->status = true;
    }
    return *this;
}

// postfix increment
Masa Masa::operator++(int){
    Masa copie = *this;
    if(this->nrPersoane < this->capacitate){
        this->nrPersoane++;
        this->status = true;
    }

    return copie;
}

// prefix decrement
Masa& Masa::operator--(){
    if(this->nrPersoane > 0 ){
        this->nrPersoane--;
        if(this->nrPersoane == 0)
            this->status = false;
    }

    return *this;
}

// postfix decrement
Masa Masa::operator--(int){
    Masa copie = *this;
    if(this->nrPersoane > 0 ){
        this->nrPersoane--;
        if(this->nrPersoane == 0)
            this->status = false;
    }

    return copie;
}

// subscript operator
char& Masa::operator[](int index) {
    if (!this->observatii.empty() && index >= 0 && index < (int)this->observatii.size()) {
        return this->observatii[index];
    }
    static char eroare = '\0';
    return eroare;
}

std::vector<Masa*> Masa:: catalogMese;


class Comanda : public IObject{

    private:
        int idMasa;
        Produs* produse;
        int nrP;
        float total;

        // list chosen for active orders: frequent insertions and deletions in O(1)
        static std::list<Comanda*> comenzi;
        static const float TVA;

    public:

        // default constructor
        Comanda()
            : idMasa(0), nrP(0), total(0){
            
            cout<<"Comanda - constructor apelat"<<endl;
            this->produse = nullptr;
        }

        // constructor with all parameters
        Comanda(int idMasa, Produs* produse, int nrP)
            : idMasa(idMasa), nrP(nrP){

            cout<<"Comanda - constructor apelat"<<endl;
            if(produse != nullptr && nrP > 0 ){
                this->produse = new Produs[nrP];
                float suma = 0;

                for(int i = 0; i < nrP; i ++){
                    this->produse[i] = produse[i];
                    suma += produse[i].getPret();
                } 

                this->total = suma;
            }
            else{
                this->produse = nullptr;
                this->nrP = 0;
                this->total = 0;
            }
        }

        // constructor with table id and person count
        Comanda(int idMasa, int nrP)
            :idMasa(idMasa), nrP(nrP), total(0){

            cout<<"Comanda - constructor apelat"<<endl;
            if( nrP > 0 ){
                this->produse = new Produs[nrP];
            }
            else{
                this->produse = nullptr;
                this->nrP = 0;
            }
        }

        // constructor with explicit table id
        explicit Comanda(int idMasa)
            :idMasa(idMasa), nrP(0), total(0){
            
            cout<<"Comanda - constructor apelat"<<endl;
            this->produse = nullptr;
        }

        // copy constructor
        Comanda(const Comanda& sursa)
            :idMasa(sursa.idMasa), nrP(sursa.nrP), total(sursa.total){

            cout<<"Comanda - constructor de copiere apelat"<<endl;
            if(sursa.produse != nullptr && sursa.nrP > 0){
                this->produse = new Produs[sursa.nrP];

                for(int i = 0; i < sursa.nrP; i++){
                    this->produse[i] = sursa.produse[i];
                }
            }
            else{
                this->produse = nullptr;
                this->nrP = 0;
                this->total = 0;
            }
        }

        // destructor
        ~Comanda() noexcept{

            cout<<"Comanda - destructor apelat"<<endl;
            
            // deallocate products
            if(this->produse != nullptr){
                delete[] this->produse;
                this->produse = nullptr;
            }
        }

        // getter for table id
        int getIdMasa() const{
            return this->idMasa;
        }

        // setter for table id
        void setIdMasa(int idMasa_nou){
            if(idMasa_nou > 0)
                this->idMasa = idMasa_nou;
        }

        // getter for products
        Produs* getProduse() const{
            // copy product list
            if(this->produse != nullptr && this->nrP > 0){
                Produs* copie = new Produs[this->nrP];
                for(int i = 0; i < this->nrP; i++ ){
                    copie[i] = this->produse[i];
                }

                return copie;
                // return copy for safety
            }
            return nullptr;
        }

        // setter for products
        void setProduse(Produs* produse_noi, int nrP_nou){
            if(produse_noi != nullptr && nrP_nou > 0){

                // deallocate previous products
                if(this->produse != nullptr){
                    delete[] this->produse;
                }

                if(produse_noi != nullptr && nrP_nou > 0){
                    // build updated prices vector
                    this->produse = new Produs[nrP_nou];
                    this->nrP = nrP_nou;
                    float suma = 0;

                    for(int i = 0; i < nrP_nou; i++){
                        this->produse[i] = produse_noi[i];
                        suma += produse_noi[i].getPret();
                    }

                    this->total = suma;
                }
                else{
                    this->nrP = 0;
                    this->total = 0;
                }
            }
        }

        // getter for person count
        int getNrP() const{
            return this->nrP;
        }

        // getter for total
        float getTotal() const{
            return this->total;
        }

        // getter for total orders count
        static int getNrComenzi(){
            return comenzi.size();
        }

        // get order by id
        static Comanda* getComandaDupaId( int idCautat){
            if(comenzi.empty()){
                return nullptr;
            }
            auto it = std::find_if(comenzi.begin(), comenzi.end(), [idCautat](Comanda* c){
                return c->getId() == idCautat;
            });
            
            if(it != comenzi.end()){
                return *it;
            }

            return nullptr;
        }

        // add product to order 
        void adaugaProdus(const Produs& produsNou){

            Produs* temp = new Produs[this->nrP + 1];
    
            if(this->produse != nullptr){
                for(int i = 0; i < this->nrP; i++){
                    temp[i] = this->produse[i];
                }
                delete[] this->produse;
            }

            temp[this->nrP] = produsNou;
            this->produse = temp;
            this->nrP++;
            this->total += produsNou.getPret();
            // log event
            Logger::getInstance().log("Produs adaugat la comanda. Total curent: " + format2(total));
        }
        

        // remove product by name
        void stergeProdus(const std::string& numeP){
            if(this->produse == nullptr || this->nrP == 0){
                throw RestaurantException("Eroare Comanda: Comanda este deja goala! Nu exista produse de sters.");
            }
            int index = -1;
            // find first product match by name
            for(int i = 0; i < this->nrP; i++){
                if(this->produse[i].getNume() == numeP){
                    index = i;
                    break;
                }
            }

            if(index != -1){
                // single product removal clears order
                float pretSters = this->produse[index].getPret();
                if(this->nrP == 1){
                    delete[] this->produse;
                    this->produse = nullptr;
                    this->nrP = 0;
                    this->total = 0;
                }
                else{
                    // rebuild list without removed item
                    Produs* temp = new Produs[this->nrP - 1];
                    int poz = 0;
                    for(int i = 0; i < this->nrP; i++){
                        if(i != index){
                            temp[poz++] = this->produse[i];
                        }
                    }
                    delete[] this->produse;
                    this->produse = temp;
                    this->nrP--;
                    this->total -= pretSters;
                }

                cout<<"Produsul a fost sters cu succes !"<<endl;
            }
            else{
                throw ProdusInexistentException(numeP);
            }
        }

        // print order history
        static void AfisareComenzi(){
            cout<<" Numar comenzi Restaurant Galmondo: ( "<<comenzi.size()<<" ) "<<endl;

            int i = 1;
            for(Comanda* c : comenzi){
                if(c != nullptr)
                    cout<<i++<<". "<<endl<<*c<<endl;
            }
        }

        // add order to catalog 
        void adaugaInComenzi() {
            auto it = std::find(comenzi.begin(), comenzi.end(), this);

            if(it == comenzi.end()){ // prevent duplicate orders
                comenzi.push_back(this);
            }
        }

        // remove order with remove_if
        static void eliminaComanda(int idCautat) {
            if (comenzi.empty()) {
                throw RestaurantException("Eroare Comanda: Istoricul comenzilor este gol!");
            }

            bool gasit = false; 

            comenzi.remove_if([idCautat, &gasit](Comanda* c) {
                if (c->getId() == idCautat) {
                    delete c;      
                    gasit = true;  
                    return true;   
                }
                return false;
            });

            if (gasit) {
                cout << "Comanda cu ID: " << idCautat << " a fost eliminata." << endl;
            } else {
                throw RestaurantException("Eroare Comanda: Nu exista comanda cu ID-ul " + to_string(idCautat) + "!");
            }
        }

        // clear order catalog
        static void curataComenzi(){
            for(Comanda* c : comenzi){
                delete c;
            }

            comenzi.clear();
        }

        // calculate tip/tax amount
        float getValoareTVA() const{
            if(this->total > 0){
                return this->total - (this->total / (1.0f + TVA));
            }
            return 0;
        }

        // print receipt
        void afiseazaNota() const{
            cout<<"Detalii nota: "<<endl;
            if(this->nrP > 0 && this->produse != nullptr){
                for(int i = 0; i < this->nrP; i++){
                    cout<<"Produs "<<i + 1<<": "<<this->produse[i].getNume()<<"........."<<this->produse[i].getPret()<<" RON."<<endl;
                }
            }
            else{
                throw RestaurantException("Eroare Comanda: Comanda nu are produse inregistrate!");
            }

            float pretFaraTVA = this->total / (1.0f + TVA);
            float taxa = this->total - pretFaraTVA;

            cout<<"Suma neta (fara taxa) "<<pretFaraTVA<<" RON."<<endl;
            cout<<"Taxa colectata (21%) "<<taxa<<" RON."<<endl;
            cout<<"Total de plata: "<<this->total<<endl;
        }

        // apply discount
        void aplicaReducere(int procent){
            if(procent > 0 && procent <= 100){
                if(this->produse != nullptr && this->nrP > 0){
                    
                    // apply discount to item prices
                    for(int i = 0; i < this->nrP; i++){
                        float reducere = (this->produse[i].getPret() * (float)procent) / 100.0f;
                        this->produse[i].setPret( this->produse[i].getPret() - reducere);
                    }

                    // apply discount to total
                    float reducere_total = (this->total * (float)procent ) / 100.0f;
                    this->total -= reducere_total;

                    cout<<"Reducere aplicata cu succes !"<<endl;
                }
            }
        }

        // global happy hour discount using for_each
        static void HappyHour(int procent) {
            if (comenzi.empty()) {
                throw RestaurantException("Eroare Comanda: Nu exista comenzi active pentru Happy Hour!");
            }

            // apply to all orders 
            std::for_each(comenzi.begin(), comenzi.end(), [procent](Comanda* c) {
                c->aplicaReducere(procent); 
            });

            cout<<"Reducerea de "<<procent<<" % a fost aplicata pe toate comenzile active!"<<endl;
        }

        // interactive order creation
        static void createComanda(){
            int id, nr;
            cout<<"----Creare comanda noua----"<<endl;
            cout<<"Numarul mesei: "; cin>>id;
            cout<<"Numar produse: "; cin>>nr;
            
            Comanda* noua = new Comanda(id);

            if(nr > 0){
                Produs::afisareMeniu();
                for(int i = 0; i < nr; i++){
                    std::string numeP;
                    cout<<"Nume produs ["<<i+1<<"]: ";
                    cin>>ws;
                    getline(cin, numeP);
                    Produs* gasit = Produs::getProdusDinMeniu(numeP);
                    if(gasit != nullptr){
                        noua->adaugaProdus(*gasit);
                        cout<<"Produsul "<<gasit->getNume()<<" cu pretul "<<gasit->getPret()<<" RON a fost adaugat"<<endl;
                    }
                    else{
                        cout<<"Produsul "<<numeP<<" nu a fost gasit in meniu."<<endl;
                        i--;
                    }
                }
            }

            noua->adaugaInComenzi();
            cout<<"Comanda a fost inregistrata."<<endl;
            Logger::getInstance().log("S-a adaugat comanda cu ID " + 
                                  to_string(noua->getId()) + " in istoricul comenzilor.");
        }

        // toString method
        std::string toString() const override {
            std::ostringstream out;
            
            out<<"ID Comanda: "<<this->id<<endl
                <<"ID Masa: "<<to_string(this->idMasa)<<endl;
                if(this->nrP > 0 && this->produse != nullptr){
                    out<<"Lista de produse: "<<endl;
                    for(int i = 0; i <= this->nrP - 1; i++){
                        out<<this->produse[i].getNume()<<" - "<<format2(this->produse[i].getPret())<<" RON"<<endl;
                    }
                }
                else{
                    out<<"Comanda nu are produse inregistrate"<<endl;
                }

                out<<"Numar Produse: "<<to_string(this->nrP)<<endl
                    <<"Total de plata: "<<format2(this->total)<<" RON"<<endl;
                
            return out.str();
        }

        // assignment operator
        Comanda& operator=(const Comanda& sursa){
            if(this != &sursa){
                this->idMasa = sursa.idMasa;
                this->nrP = sursa.nrP;
                this->total = sursa.total;

                if(this->produse != nullptr){
                    delete[] this->produse;
                    this->produse = nullptr;
                }

                if(sursa.produse != nullptr && sursa.nrP > 0){
                    this->produse = new Produs[sursa.nrP];
                    for(int i = 0; i < sursa.nrP; i++){
                        this->produse[i] = sursa.produse[i];
                    }
                }
                else{
                    this->produse = nullptr;
                }
            }

            return * this;
        }

        // float conversion operator
        explicit operator float() const{
            return this->total;
        }

        // logical not operator
        bool operator!() const {
            return (this->nrP == 0 || this->produse == nullptr);
        }

        // input operator declaration
        friend istream& operator>>(istream& in, Comanda& c);

        // arithmetic operators
        friend Comanda operator+(const Comanda& c, int cantitate);
        friend Comanda operator+(int cantitate, const Comanda& c);
        friend Comanda operator-(const Comanda& c, int cantitate);
        
        // comparison operators
        friend bool operator<(const Comanda& c1,const Comanda& c2);
        friend bool operator==(const Comanda& c1,const Comanda& c2);

        // increment operators
        Comanda& operator++(); // prefix increment
        Comanda operator++(int); // postfix increment
        Comanda& operator--(); // prefix decrement
        Comanda operator--(int); // postfix decrement

        // subscript operator
        Produs& operator[](int index);

};

// input operator
istream& operator>>(istream& in, Comanda& c){
    cout<<"Introduceti numarul mesei: ";
    in>>c.idMasa;
    
    if(c.produse != nullptr ){
        delete[] c.produse;
        c.produse = nullptr;
    }

    cout<<"Introduceti numarul de produse: ";
    in>>c.nrP;

    if(c.nrP > 0){
        c.produse = new Produs[c.nrP];
        float suma = 0;
        std::string buffer;
        float pretTemp;

        for(int i = 0; i< c.nrP; i++){
            cout<<"Nume produs ["<<i<<"]: ";
            in>>ws;
            getline(in, buffer);
            cout<<"Pret produs ["<<i<<"]: ";
            in>>pretTemp;
            c.produse[i].setNume(buffer);
            c.produse[i].setPret(pretTemp);
            suma += pretTemp;
        }
        c.total = suma;
    }
    else{
        c.nrP = 0;
        c.total = 0;
    }

    return in;
}

// addition operator
Comanda operator+(const Comanda& c, int cantitate){
    Comanda copie = c;
    if(cantitate > 0)
        copie.total += cantitate;
    
    return copie;
}

// commutative addition
Comanda operator+(int cantitate, const Comanda& c){
    return c + cantitate;
}

// subtraction operator 
Comanda operator-(const Comanda& c, int cantitate){
    Comanda copie = c;
    if(copie.total >= cantitate)
        copie.total -= cantitate;

    return copie;

}

// total comparison
bool operator<(const Comanda& a, const Comanda& b){
    return a.total < b.total;
}

// total equality
bool operator==(const Comanda& a, const Comanda& b){
    return a.total == b.total;
}

// prefix increment
Comanda& Comanda::operator++(){
    this->total += 10;
    return *this;
}

// postfix increment
Comanda Comanda::operator++(int){
    Comanda copie = *this;
    this->total += 10;
    return copie;
}

// prefix decrement
Comanda& Comanda::operator--(){
    if(this->total >= 10)
        this->total -= 10;
    return *this;
}

// postfix decrement
Comanda Comanda::operator--(int){
    Comanda copie = *this;
    if(this->total >= 10)
        this->total -= 10;
    return copie;
}

// subscript operator
Produs& Comanda::operator[](int index) {
    if (this->produse != nullptr &&  index >= 0 && index < this->nrP ) {
        return this->produse[index];
    }
    static Produs eroare ;
    return eroare;
}

std::list<Comanda*> Comanda::comenzi;
const float Comanda::TVA = 0.21f;


// modify person details implementation 
void Persoana::modificaDate() {

    if(personal.empty()){
        cout<<"Nu exista persoane inregistrate."<<endl;
        return;
    }

    std::string numeC;
    cout<<"Numele persoanei cautate: "<<endl;
    cin>>ws; getline(cin,numeC);

    bool gasit = false;

    for(auto const& [id, p] : personal){
        if(p != nullptr && p->nume == numeC) {
            gasit = true;

            // downcast to chef
            Bucatar* b = dynamic_cast<Bucatar*>(p);
            if(b != nullptr){
                int opt;
                cout<<"Persoana identificata ca bucatar."<<endl;
                cout<<"1. Schimba specialitatea"<<endl;
                cout<<"2. Schimba anii de experienta"<<endl;
                cout<<"3. Acorda bonus"<<endl;
                cout<<"0. Inapoi"<<endl;
                cout<<"Selectie: "<<endl;

                while(!(cin>>opt)){
                    cout<<"Introduceti un numar valid"<<endl;
                    cin.clear(); cin.ignore(1000, '\n');
                }

                switch(opt){
                    case 1:{
                        std::string spec;
                        cout<<"Specialitatea noua: "<<endl;
                        cin>>ws; getline(cin, spec);
                        b->setSpecializare(spec);
                        cout<<"Specializare actualizata cu succes."<<endl;
                        break;
                    }
                    case 2:{
                        int ani;
                        cout<<"Ani experienta noi: "<<endl;
                        cin>>ani;
                        b->setAniExperienta(ani);
                        cout<<"Ani experienta actualizati cu succes."<<endl;
                        break;
                    }
                    case 3:{
                        float suma;
                        cout<<"Suma bonus: "<<endl;
                        cin>>suma;
                        b->acordaBonus(suma);
                        cout<<"Bonus acordat. Salariu nou: "<<b->calculeazaSalariu()<<" RON."<<endl;
                        break;
                    }
                    
                    case 0: break;
                    default: cout<<"Optiune invalida."<<endl; 
                }
                return;
            }
            
            // downcast to waiter
            Ospatar* o = dynamic_cast<Ospatar*>(p);
            if(o != nullptr){
                int opt;
                cout<<"Persoana identificata ca ospatar."<<endl;
                cout<<"1. Schimba etajul"<<endl;
                cout<<"2. Modifica schimbul"<<endl;
                cout<<"3. Acorda bonus"<<endl;
                cout<<"0. Inapoi"<<endl;
                cout<<"Selectie: "<<endl;

                while(!(cin>>opt)){
                    cout<<"Introduceti un numar valid"<<endl;
                    cin.clear(); cin.ignore(1000, '\n'); 
                }

                switch(opt){
                    case 1:{
                        int et;
                        cout<<"Etaj nou: "<<endl;
                        cin>>et;
                        o->setEtaj(et);
                        cout<<"Etaj actualizat cu succes."<<endl;
                        break;
                    }
                    case 2:{
                        int schimb;
                        cout<<"Schimb nou: "<<endl;
                        cin>>schimb;
                        o->setSchimb(schimb);
                        cout<<"Schimb actualizat cu succes."<<endl;
                        break;
                    }
                    case 3:{
                        float suma;
                        cout<<"Suma bonus: "<<endl;
                        cin>>suma;
                        o->acordaBonus(suma);
                        cout<<"Bonus acordat. Salariu nou: "<<o->calculeazaSalariu()<<" RON."<<endl;
                        break;
                    }
                    
                    case 0: break;
                    default: cout<<"Optiune invalida."<<endl; 
                }
                return;
            }

            // downcast to administrator
            Administrator* a = dynamic_cast<Administrator*>(p);
            if(a != nullptr){
                int opt;
                cout<<"Persoana identificata ca administrator."<<endl;
                cout<<"1. Schimba profitul"<<endl;
                cout<<"2. Schimba numele restaurantului"<<endl;
                cout<<"3. Schimba anii de functionare"<<endl;
                cout<<"0. Inapoi"<<endl;
                cout<<"Selectie: "<<endl;

                while(!(cin>>opt)){
                    cout<<"Introduceti un numar valid"<<endl;
                    cin.clear(); cin.ignore(1000, '\n');
                }

                switch(opt){
                    case 1:{
                        float profit;
                        cout<<"Profit nou: "<<endl;
                        cin>>profit;
                        a->setProfit(profit);
                        cout<<"Profit actualizat cu succes."<<endl;
                        break;
                    }
                    case 2:{
                        std::string nume;
                        cout<<"Nume nou: "<<endl;
                        cin>>ws; getline(cin, nume);
                        a->setNumeRestaurant(nume);
                        cout<<"Numele restaurantului actualizat cu succes."<<endl;
                        break;
                    }
                    case 3:{
                        int ani;
                        cout<<"Ani de functionare noi: "<<endl;
                        cin>>ani;
                        a->setAniFunctionare(ani);
                        cout<<"Ani de functionare actualizati cu succes."<<endl;
                        break;
                    }
                    
                    case 0: break;
                    default: cout<<"Optiune invalida."<<endl; 
                }
                return;
            }

            // generic employee fallback
            Angajat* ang = dynamic_cast<Angajat*>(p);
            if(ang != nullptr){
                int opt;
                cout<<"Persoana identificata ca angajat generic."<<endl;
                cout<<"1. Schimba salariul"<<endl;
                cout<<"2. Acorda bonus"<<endl;
                cout<<"0. Inapoi"<<endl;
                cout<<"Selectie: "<<endl;

                while(!(cin>>opt)){
                    cout<<"Introduceti un numar valid"<<endl;
                    cin.clear(); cin.ignore(1000, '\n'); 
                }

                switch(opt){
                    case 1:{
                        float sal;
                        cout<<"Salariu nou: "<<endl;
                        cin>>sal;
                        ang->setSalariu(sal);
                        cout<<"Salariu actualizat cu succes."<<endl;
                        break;
                    }
                    case 2:{
                        float suma;
                        cout<<"Suma bonus: "<<endl;
                        cin>>suma;
                        ang->acordaBonus(suma);
                        cout<<"Bonus acordat. Salariu nou: "<<ang->calculeazaSalariu()<<" RON."<<endl;
                        break;
                    }
                    
                    case 0: break;
                    default: cout<<"Optiune invalida."<<endl; 
                }
                return;
            }

        }

    }
    if(!gasit){
        cout<<"Persoana nu a fost gasita in sistem."<<endl;
    }
}



// int to string conversion for feedback register
std::string conversieSteleInText(int stele) {
    switch(stele) {
        case 5: return "Excelent (5 stele) - Clientul a fost incantat!";
        case 4: return "Foarte bine (4 stele) - Mancare buna, servire ok.";
        case 3: return "Bine (3 stele) - Experienta medie.";
        case 2: return "Slab (2 stele) - Au existat probleme.";
        case 1: return "Foarte slab (1 stea) - Client dezamagit.";
        default: return "Nota invalida (" + to_string(stele) + " stele).";
    }
}

// string to int conversion for feedback register
int conversieTextInStele(std::string calificativ) {
    if(calificativ == "Excelent") return 5;
    if(calificativ == "Foarte bine") return 4;
    if(calificativ == "Bine") return 3;
    if(calificativ == "Slab") return 2;
    if(calificativ == "Foarte slab") return 1;
    
    // safety fallback
    return 0; 
}


// abstract factory

// abstract product interfaces

// product type: main course
class IFelPrincipal : public IObject {
public:
    virtual void pregatesteMancarea() const = 0;
};

// product type: beverage
class IBautura : public IObject {
public:
    virtual void toarnaInPahar() const = 0;
};

// concrete products

// regular main course
class BurgerClasic : public IFelPrincipal {
public:
    void pregatesteMancarea() const override {
        cout<<"[Bucatarie] Se pregateste Burger Clasic cu vita si cascaval."<<endl;
    }

    // override toString for IObject
    string toString() const override { return "Produs: Burger Clasic"; }
};


// regular beverage
class Milkshake : public IBautura {
public:
    void toarnaInPahar() const override {
        cout<<"[Bar] Se toarna Milkshake ."<<endl;
    }

    // override toString for IObject
    string toString() const override { return "Bautura: Milkshake cu lapte si inghetata de vanilie."; }
};

// vegan main course
class BurgerVegan : public IFelPrincipal {
public:
    void pregatesteMancarea() const override {
        cout<<"[Bucatarie] Se pregateste Burger Vegan cu chiftea de naut."<<endl;
    }

    // override toString for IObject
    string toString() const override { return "Produs: Burger Vegan"; }
};


// vegan beverage
class Limonada : public IBautura {
public:
    void toarnaInPahar() const override {
        cout<<"[Bar] Se pregateste Limonada proaspata cu menta."<<endl;
    }

    // override toString for IObject
    string toString() const override { return "Bautura: Limonada"; }
};


// abstract factory interface
class IFabricaMeniu {
public:
    virtual IFelPrincipal* creazaFelPrincipal() const = 0;
    virtual IBautura* creazaBautura() const = 0;
    virtual ~IFabricaMeniu() = default;
};


// concrete factories

// standard menu factory
class FabricaStandard : public IFabricaMeniu {
public:
    IFelPrincipal* creazaFelPrincipal() const override {
        Logger::getInstance().log("Fabrica Standard: S-a creat un Burger Clasic.");
        return new BurgerClasic();
    }
    IBautura* creazaBautura() const override {
        Logger::getInstance().log("Fabrica Standard: S-a creat un Milkshake.");
        return new Milkshake();
    }
};

// vegan menu factory
class FabricaVegan : public IFabricaMeniu {
public:
    IFelPrincipal* creazaFelPrincipal() const override {
        Logger::getInstance().log("Fabrica Vegan: S-a creat un Burger Vegan.");
        return new BurgerVegan();
    }
    IBautura* creazaBautura() const override {
        Logger::getInstance().log("Fabrica Vegan: S-a creat o Limonada.");
        return new Limonada();
    }
};


// client receives factory via dependency injection
void servesteClient(const IFabricaMeniu& fabricaAlesa) {
    cout<<"--- Incepere servire client ---"<<endl;
    
    // client works with abstract interfaces
    IFelPrincipal* mancare = fabricaAlesa.creazaFelPrincipal();
    IBautura* bautura = fabricaAlesa.creazaBautura();

    mancare->pregatesteMancarea();
    bautura->toarnaInPahar();

    // deallocate combo items
    delete mancare;
    delete bautura;
}


int main() {

    
   // initialize products
    (new Produs("Pizza Margherita", 35.0f, 800, 10))->adaugaInMeniu();
    (new Produs("Paste Carbonara", 28.0f, 650, 15))->adaugaInMeniu();
    (new Produs("Salata Caesar", 22.0f, 350, 20))->adaugaInMeniu();
    (new Produs("Supa Crema Rosii", 18.0f, 250, 12))->adaugaInMeniu();
    (new Produs("Tiramisu", 20.0f, 450, 8))->adaugaInMeniu();
    (new Produs("Limonada", 12.0f, 120, 25))->adaugaInMeniu();

    // initialize tables
    (new Masa(2, 0, false, "Masa 1 - fereastra"))->adaugaInCatalog();
    (new Masa(4, 2, true,  "Masa 2 - ocupata"))->adaugaInCatalog();
    (new Masa(6, 0, false, "Masa 3 - terasa"))->adaugaInCatalog();
    (new Masa(4, 0, false, "Masa 4 - interior"))->adaugaInCatalog();

     // initialize employees
    // generic employee
    Angajat* a1 = new Angajat("Ion Popescu", 'M', 1234567890123LL, 15, 3, 1990, 3000.0f);
    a1->adaugaInCatalog();
    a1->adaugaInPersonal();

    // chefs
    Bucatar* b1 = new Bucatar("Maria Ionescu", 'F', 2345678901234LL, 10, 5, 1988, 4000.0f, "italiana", 8);
    b1->adaugaInCatalog();
    b1->adaugaInPersonal();

    Bucatar* b2 = new Bucatar("Gheorghe Popa", 'M', 3456789012345LL, 20, 7, 1992, 3500.0f, "francesa", 5);
    b2->adaugaInCatalog();
    b2->adaugaInPersonal();

    // waiters
    Ospatar* o1 = new Ospatar("Ana Dumitrescu", 'F', 4567890123456LL, 5, 9, 1995, 2800.0f, 1, 2);
    o1->adaugaInCatalog();
    o1->adaugaInPersonal();

    Ospatar* o2 = new Ospatar("Mihai Constantin", 'M', 5678901234567LL, 12, 11, 1993, 2600.0f, 2, 3);
    o2->adaugaInCatalog();
    o2->adaugaInPersonal();

    // administrator
    Administrator* ad1 = new Administrator("Alexandru Galmondo", 'M', 6789012345678LL, 1, 1, 1975, 25000.0f, "Restaurant Galmondo", 12);
    ad1->adaugaInPersonal();

    // initialize orders
    Comanda* c1 = new Comanda(1);
    Produs* pizza = Produs::getProdusDinMeniu("Pizza Margherita");
    Produs* paste = Produs::getProdusDinMeniu("Paste Carbonara");
    if(pizza) c1->adaugaProdus(*pizza);
    if(paste) c1->adaugaProdus(*paste);
    c1->adaugaInComenzi();

    Comanda* c2 = new Comanda(3);
    Produs* salata = Produs::getProdusDinMeniu("Salata Caesar");
    Produs* limonada = Produs::getProdusDinMeniu("Limonada");
    if(salata) c2->adaugaProdus(*salata);
    if(limonada) c2->adaugaProdus(*limonada);
    c2->adaugaInComenzi();

    int optiune = -1;

    Logger::getInstance().log("Sistemul Galmondo a fost pornit.");

    // instantiate template classes 
    RegistruFeedback<string, 50> pareriText;
    RegistruFeedback<int, 50> noteClienti;
    
    while(optiune != 0){

        cout<<"====================================================="<<endl;
        cout<<"       SISTEM DE GESTIUNE RESTAURANT GALMONDO        "<<endl;
        cout<<"====================================================="<<endl;
        cout<<"1. GESTIUNE MENIU RESTAURANT"<<endl;
        cout<<"2. GESTIUNE PERSONAL"<<endl;
        cout<<"3. GESTIUNE MESE"<<endl;
        cout<<"4. GESTIUNE COMENZI"<<endl;
        cout<<"5. GESTIUNE FEEDBACK"<<endl;
        cout<<"6. JURNAL SISTEM"<<endl;
        cout<<"7. SERVIRE RAPIDA"<<endl;
        cout<<"0. IESIRE"<<endl;
        cout<<"====================================================="<<endl;
        cout<<"Selectie: "<<endl;

        string inputOptiune;
        cin>>inputOptiune;
        try {
            optiune = stoi(inputOptiune);
        } catch (const exception& e) {
            cout<<"Eroare: Va rugam introduceti o cifra valida!"<<endl;
            continue; 
        }

        switch(optiune){

            case 1: {
                int opt1 = -1;
                while(opt1 != 0){
                    cout<<endl;
                    cout<<"--GESTIUNE MENIU RESTAURANT--"<<endl;
                    cout<<"1. Adauga un produs nou"<<endl;
                    cout<<"2. Sterge produs"<<endl;
                    cout<<"3. Afiseaza meniu"<<endl;
                    cout<<"4. Aplica reducere produs"<<endl;
                    cout<<"5. Afiseaza meniu sortat dupa pret "<<endl;
                    cout<<"0. INAPOI"<<endl;

                    cout<<"Selectie: "<<endl;

                    string inputOptiune1;
                    cin>>inputOptiune1;
                    try {
                        opt1 = stoi(inputOptiune1);
                    } catch (const exception& e) {
                        cout<<"Eroare: Va rugam introduceti o cifra valida!"<<endl;
                        continue; 
                    }

                    // option 1: menu products
                    try{
                        switch(opt1){
                            case 1:
                                Produs::create(); break;
                            case 2: {
                                std::string numeD;
                                cout<<"Nume produs de sters: ";
                                cin>>ws;
                                getline(cin, numeD);
                                Produs::eliminaProdus(numeD);
                                break;
                            }
                            case 3:
                                Produs::afisareMeniu(); break;
                            case 4:{
                                std::string numeR;
                                float proc;
                                cout<<"Nume produs pentru reducere: ";
                                cin>>ws;
                                getline(cin, numeR);
                                cout<<"Procent reducere: ";
                                cin>>proc;
                                Produs::aplicaReducereInMeniu(numeR, proc);
                                break;
                            }
                            case 5:{
                                Produs::afiseazaMeniuSortatDupaPret();
                                break;
                            }
                            case 0: break;
                            default: cout<<"Optiune invalida."<<endl; 
                        }
                    }
                    catch (const ProdusInexistentException& e) {
                            cout<<"[Eroare cautare produs]: "<<e.what()<<endl;
                            Logger::getInstance().log("Actiune esuata (Produs negasit): " + string(e.what()));
                    }
                    catch (const DateInvalideException& e) {
                        cout<<"[Eroare Date]: "<<e.what()<<endl;
                        Logger::getInstance().log("Actiune esuata (Date invalide): " + string(e.what()));
                    }
                    catch (const RestaurantException& e) { 
                        //"Nume invalid sau meniul este gol!"
                        cout<<"[Eroare la Meniu]: "<<e.what()<<endl;
                        Logger::getInstance().log("Eroare generica clasa Produs: " + string(e.what()));
                    }
                    catch (const std::exception& e) {
                        cout<<"[Eroare C++ STL]: "<<e.what()<<endl;
                        Logger::getInstance().log("Eroare STL in sub-meniul Produs.");
                    }
                    catch (...) {
                        cout<<"[Eroare Critica]: Ceva neasteptat s-a intamplat. Revenire la meniu..."<<endl;
                        Logger::getInstance().log("Eroare critica necunoscuta prinsa in sub-meniu.");
                    }

                }
                break;
            }

            case 2:{
                int opt2 = -1;
                while(opt2 != 0){
                    cout<<endl;
                    cout<<"GESTIUNE PERSONAL"<<endl;
                    cout<<"1. Inregistreaza angajat nou"<<endl;
                    cout<<"2. Inregistreaza bucatar nou"<<endl;
                    cout<<"3. Inregistreaza ospatar nou"<<endl;
                    cout<<"4. Inregistreaza administrator nou"<<endl;
                    cout<<"5. Elimina angajat din catalog angajati"<<endl;
                    cout<<"6. Elimina persoana din personal"<<endl;
                    cout<<"7. Afisare personal"<<endl;
                    cout<<"8. Clasament si statistici angajati"<<endl;
                    cout<<"9. Acorda bonus"<<endl;
                    cout<<"10.Istoric bonusuri angajat"<<endl;
                    cout<<"11.Operatii specifice tip"<<endl;
                    cout<<"0. INAPOI"<<endl;

                    cout<<"Selectie: "<<endl;

                    string inputOptiune2;
                    cin>>inputOptiune2;
                    try {
                        opt2 = stoi(inputOptiune2);
                    } catch (const exception& e) {
                        cout<<"Eroare: Va rugam introduceti o cifra valida!"<<endl;
                        continue; 
                    }

                    // option 2: staff management
                    try{
                        switch(opt2){
                            case 1:
                                Angajat::create(); break;
                            case 2:
                                Bucatar::create(); break;
                            case 3:
                                Ospatar::create(); break;
                            case 4:
                                Administrator::create(); break;
                            case 5:{
                                std::string numeD;
                                cout<<"Numele angajatului pentru concediere: ";
                                cin>>ws;
                                getline(cin, numeD);
                                Angajat::eliminaAngajat(numeD);
                                break;
                            }
                            case 6:{
                                std::string numeE;
                                cout<<"Numele persoanei de eliminat: ";
                                cin>>ws; getline(cin, numeE);
                                Persoana::eliminaPersoana(numeE);
                                break;
                            }
                            case 7:
                                Persoana::afisarePersonal(); break;
                            case 8:
                                Angajat::afisareStatisticiCatalog(); break;
                            case 9:{
                                std::string numeB;
                                int sumaB;
                                cout<<"Numele angajatului pentru bonus: ";
                                cin>>ws;
                                getline(cin, numeB);
                                cout<<"Bonus in LEI: ";
                                cin>>sumaB;
                                Angajat::acordaBonusNume(numeB, sumaB);
                                break;
                            }
                            case 10:{
                                std::string numeIB;
                                cout<<"Numele angajatului: ";
                                cin>>ws; getline(cin, numeIB);
                                Angajat::afiseazaIstoricBonusuriNume(numeIB);
                                break;
                            }
                            case 11:{
                                Persoana::modificaDate();
                                break;
                            }
                            case 0: break;
                            default: cout<<"Optiune invalida."<<endl; 
                        }
                    }
                    catch (const PersoanaNegasitaException& e) {
                        cout<<"[Eroare Cautare Personal]: "<<e.what()<<endl;
                        Logger::getInstance().log("Actiune Personal esuata: " + string(e.what()));
                    }
                    catch (const DateInvalideException& e) {
                        cout<<"[Eroare Date Angajat]: "<<e.what()<<endl;
                        Logger::getInstance().log("Date invalide la Personal: " + string(e.what()));
                    }
                    catch (const RestaurantException& e) { 
                        //"nu exista bonusuri" "catalog gol" etc.
                        cout<<"[Eroare ierarhie Personal]: "<<e.what()<<endl;
                        Logger::getInstance().log("Eroare generica ierarhie Personal: " + string(e.what()));
                    }
                    catch (const std::exception& e) {
                        cout<<"[Eroare C++ STL]: "<<e.what()<<endl;
                        Logger::getInstance().log("Eroare STL in sub-meniul Personal.");
                    }
                    catch (...) {
                        cout<<"[Eroare Critica]: Ceva neasteptat s-a intamplat. Revenire la meniu..."<<endl;
                        Logger::getInstance().log("Eroare critica necunoscuta prinsa in sub-meniu.");
                    }

                }
                break;
            }

            case 3: {
                int opt3 = -1;
                while(opt3 != 0){
                    cout<<endl;
                    cout<<"--GESTIUNE MESE--"<<endl;
                    cout<<"1. Stare mese terasa"<<endl;
                    cout<<"2. Adauga masa"<<endl;
                    cout<<"3. Ocupa masa"<<endl;
                    cout<<"4. Elibereaza masa"<<endl;
                    cout<<"5. Cauta mese disponibila pentru grup."<<endl;
                    cout<<"0. Inapoi"<<endl;
                    cout<<"Selectie: "<<endl;

                    string inputOptiune3;
                    cin>>inputOptiune3;
                    try {
                        opt3 = stoi(inputOptiune3);
                    } catch (const exception& e) {
                        cout<<"Eroare: Va rugam introduceti o cifra valida!"<<endl;
                        continue; 
                    }

                    // option 3: table management
                    // stack unwinding: throwing exception cleans stack and calls destructors before catch
                    try{
                        switch(opt3){
                            case 1:
                                Masa::afisareCatalog();
                                break;
                            case 2:
                                Masa::createMasa();
                                break;
                            case 3:{
                                int idO, nrO;
                                cout<<"ID masa de ocupat: ";
                                cin>>idO;
                                Masa* m = Masa::getMasaDupaId(idO);
                                if(m){
                                    cout<<"Numar persoane: ";
                                    cin>>nrO;
                                    m->ocupaMasa(nrO);
                                }
                                break;
                            }
                            case 4:{
                                int idE;
                                cout<<"Introduceti ID-ul mesei de eliberat: ";
                                cin>>idE;
                                Masa* m = Masa::getMasaDupaId(idE);
                                if(m){
                                    m->elibereazaMasa();
                                }
                                break;
                            }
                            case 5:{
                                int nrG;
                                cout<<"Numar persoane in grup: ";
                                cin>>nrG;
                                cout<<"Mese disponibile pentru acest grup: ";
                                Masa::cautaMeseLibere(nrG);
                                break;
                                
                            }
                            case 0: break;
                            default: cout<<"Optiune invalida."<<endl; 
                        }
                    }
                    catch (const CapacitateMasaException& e) {
                        cout<<"[Eroare Capacitate Masa]: "<<e.what()<<endl;
                        Logger::getInstance().log("Actiune esuata (Capacitate depasita): " + string(e.what()));
                    }
                    catch (const RestaurantException& e) {
                        // handle table exceptions
                        cout<<"[Eroare la Mese]: "<<e.what()<<endl;
                        Logger::getInstance().log("Eroare actiune Masa: " + string(e.what()));
                    }
                    catch (const std::exception& e) {
                        // catch invalid input conversion errors
                        cout<<"[Eroare Input/Sistem]: "<<e.what()<<endl;
                        Logger::getInstance().log("Eroare STL in sub-meniul Mese.");
                    }
                    catch (...) {
                        cout<<"[Eroare Critica]: Ceva neasteptat s-a intamplat. Revenire la meniu..."<<endl;
                        Logger::getInstance().log("Eroare critica necunoscuta prinsa in sub-meniu.");
                    }
                }
                break;
            }

            case 4: {
                int opt4 = -1;
                while(opt4 != 0){
                    cout<<endl;
                    cout<<"GESTIUNE COMENZI"<<endl;
                    cout<<"1. Afisare comenzi"<<endl;
                    cout<<"2. Adauga comanda"<<endl;
                    cout<<"3. Sterge comanda"<<endl;
                    cout<<"4. Adauga produs in comanda"<<endl;
                    cout<<"5. Sterge produs din comanda"<<endl;
                    cout<<"6. Aplica reducere"<<endl;
                    cout<<"7. Vizualizare nota de plata"<<endl;
                    cout<<"8. Aplica reducere pe toate comenzile ( HAPPY HOUR )"<<endl;
                    cout<<"0. INAPOI"<<endl;
                    cout<<"Selectie: "<<endl;

                    string inputOptiune4;
                    cin>>inputOptiune4;
                    try {
                        opt4 = stoi(inputOptiune4);
                    } catch (const exception& e) {
                        cout<<"Eroare: Va rugam introduceti o cifra valida!"<<endl;
                        continue; 
                    }

                    // option 4: orders management
                    try{
                        switch(opt4){
                            case 1:{
                                Comanda::AfisareComenzi();
                                break;
                            }
                            case 2:{
                                Comanda::createComanda();
                                break;
                            }
                            case 3:{
                                int idSters;
                                cout<<"Introduceti ID-ul comenzii de sters: "<<endl;
                                cin>>idSters;
                                Comanda::eliminaComanda(idSters);
                                break;
                            }
                            case 4:{
                                int idCmd;
                                cout<<"Introduceti ID-ul comenzii: "<<endl;
                                cin>>idCmd;
                                Comanda* c = Comanda::getComandaDupaId(idCmd);
                                if(c != nullptr){
                                    std::string numeP;
                                    Produs::afisareMeniu();
                                    cout<<"Numele produsului de adaugat: "<<endl;
                                    cin>>ws; getline(cin, numeP);
                                    Produs* gasit = Produs::getProdusDinMeniu(numeP);

                                    if(gasit != nullptr){
                                        c->adaugaProdus(*gasit);
                                        cout<<"Produs adaugat. Total curent: "<<c->getTotal()<<" RON."<<endl;
                                    }
                                    else{
                                        cout<<"Produsul nu exista in meniu"<<endl;
                                    }
                                }
                                else{
                                    cout<<"Comanda nu a fost gasita"<<endl;
                                }
                                break;
                            }
                            
                            case 5:{
                                int idCmd;
                                cout<<"Introduceti ID-ul comenzii: "<<endl;
                                cin>>idCmd;
                                Comanda* c = Comanda::getComandaDupaId(idCmd);

                                if(c != nullptr){
                                    if(!(*c)){ // check if order is active
                                        cout<<"Comanda este goala."<<endl;
                                    }
                                    else{
                                        std::string numeS;
                                        cout<<*c<<endl;
                                        cout<<"Numele produsului de sters din comanda: "<<endl;
                                        cin>>ws; getline(cin, numeS);
                                        c->stergeProdus(numeS);
                                    }
                                }
                                else{
                                    cout<<"Comanda nu a fost gasita"<<endl;
                                }
                                break;
                            }
                            case 6:{
                                int idCmd;
                                cout<<"Introduceti ID-ul comenzii: "<<endl;
                                cin>>idCmd;
                                Comanda* c = Comanda::getComandaDupaId(idCmd);

                                if(c != nullptr){
                                    int procC;
                                    cout<<"Introduceti procentul reducerii: "<<endl;
                                    cin>>procC;
                                    c->aplicaReducere(procC);
                                }
                                else{
                                    cout<<"Comanda nu a fost gasita"<<endl;
                                }
                                break;
                            }
                            case 7:{
                                int idCmd;
                                cout<<"Introduceti ID-ul comenzii: "<<endl;
                                cin>>idCmd;
                                Comanda* c = Comanda::getComandaDupaId(idCmd);

                                if(c != nullptr){
                                    c->afiseazaNota();
                                }
                                else{
                                    cout<<"Comanda nu a fost gasita"<<endl;
                                }
                                break;
                            }
                            case 8:{
                                    int procH;
                                    string inputProcH;
                                    cout<<"Introduceti procentul pentru Happy Hour: ";
                                    cin>>ws; getline(cin, inputProcH);
                                    procH = stoi(inputProcH);
                                    
                                    Comanda::HappyHour(procH);
                                    break;
                            }
                            case 0: break;
                            default: cout<<"Optiune invalida."<<endl; 
                        }
                    }
                    catch (const ProdusInexistentException& e) {
                        cout<<"[Eroare Produs in Comanda]: "<<e.what()<<endl;
                        Logger::getInstance().log("Eroare Produs (Comanda): " + string(e.what()));
                    }
                    catch (const DateInvalideException& e) {
                        cout<<"[Eroare Date Comanda]: "<<e.what()<<endl;
                        Logger::getInstance().log("Date invalide (Comanda): " + string(e.what()));
                    }
                    catch (const RestaurantException& e) {
                        // handle order exceptions
                        cout<<"[Eroare la Comenzi]: "<<e.what()<<endl;
                        Logger::getInstance().log("Eroare actiune Comanda: " + string(e.what()));
                    }
                    catch (const std::exception& e) {
                        cout<<"[Eroare Input/Sistem]: "<<e.what()<<endl;
                        Logger::getInstance().log("Eroare STL in sub-meniul Comenzi.");
                    }
                    catch (...) {
                        cout<<"[Eroare Critica]: Ceva neasteptat s-a intamplat. Revenire la meniu..."<<endl;
                        Logger::getInstance().log("Eroare critica necunoscuta prinsa in sub-meniu.");
                    }
                }
                break;
            } 

            case 5: {
                int opt5 = -1;
                while(opt5 != 0){
                    cout<<endl;
                    cout<<"GESTIUNE FEEDBACK"<<endl;
                    cout<<"1. Adauga recenzie text"<<endl;
                    cout<<"2. Adauga nota (1-5)"<<endl;
                    cout<<"3. Adauga nota din text "<<endl;
                    cout<<"4. Genereaza Raport Note "<<endl;
                    cout<<"5. Genereaza Raport Text "<<endl;
                    cout<<"6. Afiseaza Status Registre"<<endl;
                    cout<<"0. INAPOI"<<endl;
                    cout<<"Selectie: ";

                    string inputOptiune5;
                    cin>>inputOptiune5;
                    try {
                        opt5 = stoi(inputOptiune5);
                    } catch (const exception& e) {
                        cout<<"Eroare: Va rugam introduceti o cifra valida!"<<endl;
                        continue; 
                    }

                    // option 5: customer feedback
                    try {
                        switch(opt5){
                            case 1: {
                                string feedbackNou;
                                cout<<"Introduceti recenzia: ";
                                cin>>ws; getline(cin, feedbackNou);
                                
                                pareriText.adaugaFeedback(feedbackNou);
                                cout<<"Recenzie adaugata cu succes!"<<endl;
                                break;
                            }
                            case 2: {
                                int nota;
                                string inputNota;
                                cout<<"Acordati o nota (1-5): ";
                                cin>>ws; getline(cin, inputNota);
                                nota = stoi(inputNota);
                                
                                noteClienti.adaugaFeedback(nota);
                                cout<<"Nota adaugata cu succes!"<<endl;
                                break;
                            }
                            case 3: {
                                string calificativ;
                                cout<<"Introduceti calificativ (Excelent/Foarte bine/Bine/Slab/Foarte slab): ";
                                cin>>ws; getline(cin, calificativ);
                                
                                // template method inside template class
                                noteClienti.adaugaFeedbackConvertit<string>(calificativ, conversieTextInStele);
                                cout<<"Calificativ convertit si inregistrat ca nota!"<<endl;
                                break;
                            }
                            case 4: {
                                cout<<"Raport Note"<<endl;
                                // use template specialization for ratings
                                genereazaRaport(noteClienti.getIstoric()); 
                                break;
                            }
                            case 5: {
                                cout<<"Raport Text"<<endl;
                                // use template specialization for reviews
                                genereazaRaport(pareriText.getIstoric());
                                break;
                            }
                            case 6: {
                                cout<<"Status Registru Text"<<endl;
                                afiseazaStatusRegistru(pareriText);
                                cout<<"Status Registru Note"<<endl;
                                afiseazaStatusRegistru(noteClienti);
                                break;
                            }
                            case 0: break;
                            default: cout<<"Optiune invalida."<<endl; 
                        }
                    }
                    catch (const overflow_error& e) {
                        // memory limit exception
                        cout<<"[Eroare Limita Memorie]: "<<e.what()<<endl;
                        Logger::getInstance().log("Eroare Feedback: " + string(e.what()));
                    }
                    catch (const std::exception& e) {
                        cout<<"[Eroare Sistem]: "<<e.what()<<endl;
                        Logger::getInstance().log("Eroare STL in sub-meniul Feedback.");
                    }
                    catch (...) {
                        cout<<"[Eroare Critica]: Ceva neasteptat s-a intamplat. Revenire la meniu..."<<endl;
                        Logger::getInstance().log("Eroare critica necunoscuta prinsa in sub-meniu.");
                    }
                }
                break;
            }


            case 6: {
                // option 6: logger events
                try {
                    cout<<"JURNAL SISTEM "<<endl;
                    Logger::getInstance().afiseazaJurnal();
                }
                catch (const exception& e) {
                    cout<<"[Eroare Jurnal]: "<<e.what()<<endl;
                }
                catch (...) {
                    cout<<"[Eroare Critica]: Ceva neasteptat s-a intamplat. Revenire la meniu..."<<endl;
                    Logger::getInstance().log("Eroare critica necunoscuta prinsa in sub-meniu.");
                }
                break;
            }


            case 7:{ 
                int tipClient = -1;
                cout<<"SERVIRE RAPIDA"<<endl;
                cout<<"Clientul prefera meniul:"<<endl;
                cout<<"1. Standard (Burger Clasic & Milkshake)"<<endl;
                cout<<"2. Vegan (Burger Vegan & Limonada)"<<endl;
                cout<<"Alegere: ";

                string inputOptiune7;
                cin>>inputOptiune7;
                try {
                    tipClient = stoi(inputOptiune7);
                } 
                catch (const exception& e) {
                    cout<<"Eroare: Va rugam introduceti o cifra valida!"<<endl;
                    continue; 
                }

                // option 7: fast service combo 
                try {
                    // switch combo by swapping factory parameter
                    if(tipClient == 1){
                        FabricaStandard fabricaStd;
                        servesteClient(fabricaStd); 
                    } 
                    else if(tipClient == 2){
                        FabricaVegan fabricaVeg;
                        servesteClient(fabricaVeg); 
                    } 
                    else {
                        cout<<"Eroare: Va rugam alegeti 1 sau 2."<<endl;
                    }
                }
                catch (const exception& e) {
                    cout<<"A aparut o problema la servire: "<<e.what()<<endl;
                }
                catch (...) {
                    cout<<"[Eroare Critica]: Ceva neasteptat s-a intamplat. Revenire la meniu..."<<endl;
                    Logger::getInstance().log("Eroare critica necunoscuta prinsa in sub-meniu.");
                }
                break;
            }

            case 0:{
                cout<<"La revedere!"<<endl;
                break;
            }

            default:
            cout<<"optiune invalida"<<endl;

        }   
    
    }
    Produs::curataMeniu();
    Angajat::curataCatalog();
    Masa::curataCatalog();
    Comanda::curataComenzi();
    Persoana::curataPersonal();

    cout<<"Sistemul s-a inchis. Toate resuresele au fost eliberate."<<endl;

    return 0;
    
}
      

