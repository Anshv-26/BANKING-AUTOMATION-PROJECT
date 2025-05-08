//Banking System
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<map>
#include<string>
using namespace std;
#define MIN_BALANCE 500

class InsufficientFunds{};

class Account
{
private:
    int accountNumber;
    string password;
    string firstName;
    string lastName;
    int actype;  // 0 for admin 1 for saving and 2 for current and 3 for loan
    bool debitCardActivated;//1 is yes 0 is No
    bool textMessageActivated;//1 is yes 0 is No
    bool netBankingActivated;//1 is yes 0 is No

    static int NextAccountNumber;

protected:
	float balance;
 
public:
    Account();

    int getAccNo(){return accountNumber;}
    
    string getFirstName(){return firstName;}
    
    string getLastName(){return lastName;}
    
    float getBalance(){return balance;}
    
    void setPassword(string pwd){
        password = pwd;
    }
    
    string getPassword(){
        return password;
    }
    
    bool equals(string pwd){
        return password==pwd;
    }
    
    void Deposit(float amount){
        balance+=amount;
        cout<<"Updated balance = "<<balance<<endl;
    }
    
    void Withdraw(float amount);

    int getActype(){
        return actype;
    }
    
    void setActype(int ac){
        actype = ac;
    }
    
    bool isDebitCardActivated(){
        return debitCardActivated;
    }
    
    bool isTextMessageActivated(){
        return textMessageActivated;
    }
    
    bool isNetBankingActivated(){
        return netBankingActivated;
    }
    
    void setDebitCardActivated(bool act){
        debitCardActivated = act;
    }
    
    void setNetBankingActivated(bool act){
        netBankingActivated = act;
    }
    
    void setTextMessageActivated(bool act){
        textMessageActivated = act;
    }
    
    static void setLastAccountNumber(int accountNumber);
    
    static int getLastAccountNumber();
    
    friend ofstream & operator<<(ofstream &ofs,Account &acc);
    
    friend ifstream & operator>>(ifstream &ifs,Account &acc);
    
    friend ostream & operator<<(ostream &os,Account &acc);
    
    void inputData();
};

Account::Account()
{
    NextAccountNumber++;
    accountNumber=NextAccountNumber;
}

void Account::Withdraw(float amount)
{
	 if(balance-amount<MIN_BALANCE)
	 	throw InsufficientFunds();
	 balance-=amount;
	 cout<<"Updated balance = "<<balance<<endl;
}

void Account::setLastAccountNumber(int accountNumber)
{
 NextAccountNumber=accountNumber;
}

int Account::getLastAccountNumber()
{
 return NextAccountNumber;
}

ofstream& operator<<(ofstream &ofs,Account &acc)
{
    ofs<<acc.accountNumber<<endl;
    ofs<<acc.firstName<<endl;
    ofs<<acc.lastName<<endl;
    ofs<<acc.balance<<endl;
    ofs<<acc.password<<endl;
    return ofs;
}

ifstream& operator>>(ifstream &ifs,Account &acc)
{
    ifs>>acc.accountNumber;
    ifs>>acc.firstName;
    ifs>>acc.lastName;
    ifs>>acc.balance;
    ifs>>acc.password;
    return ifs;
}
ostream& operator<<(ostream &os,Account &acc)
{
    os<<"First Name:"<<acc.getFirstName()<<endl;
    os<<"Last Name:"<<acc.getLastName()<<endl;
    os<<"Account Number:"<<acc.getAccNo()<<endl;
    os<<"Balance:"<<acc.getBalance()<<endl;
    return os;
}
void Account::inputData()
{
	cout<<"Enter First Name:";
	cin>>firstName;
 	cout<<"Enter Last Name:";
	cin>>lastName;
 	cout<<"Opening Balance:";
	cin>>balance;
    cout<<"Enter the entry password for your account ";
	cin>>password;
    cout<<"Enter account type(1 = savings, 2 = current): ";
	cin>>actype;
}

int Account::NextAccountNumber=0;

class Loan: public Account{
	float roi;
	public:
		float getRoi(){
			return roi;
		}
		void setRoi(float roi){
			this->roi = roi;
		}
};

class VehicleLoan : public Loan{
	string regnno;
	public:
		string getRegnno(){
			return regnno;
		}
		void setRegnno(string regnno){
			this->regnno = regnno;
		}
};

class HomeLoan : public Loan{
	string address;
	public:
		string getAddress(){
			return address;
		}
		void setAddress(string address){
			this->address = address;
		}
};

class Bank
{
private:
 map<int,Account> accounts;

public:
 Bank();
 Account OpenAccount();
 void searchByAccountNumber(int accno);
 //void searchByAccountType(int act);
 void balanceEnquiry(int accountNumber);
 Account Deposit(int accountNumber,float amount);
 Account Withdraw(int accountNumber,float amount);
 void CloseAccount(int accountNumber);
 void ShowAllAccounts();
 bool login(int id, string pwd);
 ~Bank();
};

bool Bank::login(int id, string pwd){
	if(accounts.find(id)!=accounts.end()){
		if(accounts[id].getPassword()==pwd){
			return true;
		}else 
			cout<<"Invalid password\n";
		}else
		cout<<"Invalid ID\n";
	return false;
}


Account Bank::OpenAccount()
{
 ofstream outfile;
 Account account;
 account.inputData();
 accounts.insert(pair<int,Account>(account.getAccNo(),account));

 outfile.open("Bank.data", ios::trunc);

 map<int,Account>::iterator itr;
 for(itr=accounts.begin();itr!=accounts.end();itr++)
 {
 outfile<<itr->second;
 }
 outfile.close();
 return account;
}

void Bank::balanceEnquiry(int id)
{
	 map<int,Account>::iterator itr=accounts.find(id);
	 cout<<"Balance  = "<< itr->second.getBalance();
}

void Bank::searchByAccountNumber(int id)
{
 if(accounts.find(id)!=accounts.end()){
	 map<int,Account>::iterator itr=accounts.find(id);
	 cout<< itr->second;
 }else
	cout<<"Invalid account number";
}

Account Bank::Deposit(int accountNumber,float amount)
{
 map<int,Account>::iterator itr=accounts.find(accountNumber);
 
 itr->second.Deposit(amount);
 return itr->second;
}

Account Bank::Withdraw(int accountNumber,float amount)
{
 map<int,Account>::iterator itr=accounts.find(accountNumber);
 itr->second.Withdraw(amount);
 return itr->second;
}

void Bank::CloseAccount(int accountNumber)
{
 map<int,Account>::iterator itr=accounts.find(accountNumber);
 cout<<"Account Deleted"<<itr->second;
 accounts.erase(accountNumber);
}

void Bank::ShowAllAccounts()
{
 map<int,Account>::iterator itr;
 for(itr=accounts.begin();itr!=accounts.end();itr++)
 {
 cout<<"Account "<<itr->first<<endl<<itr->second<<endl;
 }
}

Bank::~Bank()
{
    ofstream outfile;
    outfile.open("Bank.data", ios::trunc);

    map<int,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++)
    {
    outfile<<itr->second;
    }
    outfile.close();
}

Bank::Bank()
{
     Account a1;
	 string pwd;
	 ifstream infile;
	 infile.open("Bank.data", ios::in);
	if(!infile)
	 {
		 cout<<"Error in Opening! File Not Found!!"<<endl;
		 ofstream outfile;
		 outfile.open("Bank.data", ios::out);
		 cout<<"Enter admin password ";
		 cin>>pwd;
		 a1.setPassword(pwd);
		 outfile<<a1;
		 outfile.flush();
		 outfile.close();
		 infile.open("Bank.data", ios::in);
	 }
	 while(!infile.eof())
	 {
		 infile>>a1;
		 accounts.insert(pair<int,Account>(a1.getAccNo(),a1));
	 }
	 Account::setLastAccountNumber(a1.getAccNo());
	
	 infile.close();
}

int main()
{
 Bank b;
 int id;
 int actype;
 string fname;
 int accountNumber;
 float amount;
 string pwd1;
 int choice;

 do{
    cout<<"\n\tSelect one option below ";
    cout<<"\n\t1. Login";
    cout<<"\n\t0. Quit";
    cout<<"\nEnter your choice: ";
    cin>>choice;
    switch(choice)
    {
        case 1:
            cout<<"Enter Id ";
            cin>>id;
            cout<<"Enter Password: ";
            cin>>pwd1;
            
            if(!b.login(id, pwd1)){
                return 1;
            }
            cout<<endl<<"Congratulations!! Login successful"<<endl;
            break;
        case 0: 
            exit(0);
        default:
            cout<<"\nEnter correct choice";
            exit(0);
    }
}while(choice>1 || choice<0);

 cout<<"\n\n\n***Banking System***"<<endl;
 
 do {
    cout<<"\n\tSelect one option below ";
    if(id==1){
        cout<<"\n\t1 Open a new Account";
        cout<<"\n\t2 Show All Accounts";
        cout<<"\n\t3 Search Account By ID ";
    }else{
        cout<<"\n\t1. Balance Enquiry";
        cout<<"\n\t2 Deposit";
        cout<<"\n\t3 Withdrawal";
    }
    cout<<"\n\t0 Quit";
    cout<<"\nEnter your choice: ";
    cin>>choice;
    switch(choice)
    {
        case 1:
            if(id==1){
                b.OpenAccount();
                cout<<endl<<"Congratulations! Account is Created"<<endl;
                
            }else{
                b.balanceEnquiry(id);
            }
            break;
        case 2:
            if(id==1){
                b.ShowAllAccounts();
            }else{
                cout<<"Enter the amount to deposit ";
                cin>>amount;
                b.Deposit(id,amount);
            }
            break;
        case 3:
            if(id==1){
                cout<<"Enter Account Number:";
                cin>>accountNumber;
            }
            break;
        case 4:
            cout<<"Enter Account Number:";
            cin>>accountNumber;
            cout<<"Enter Balance:";
            cin>>amount;
            b.Withdraw(accountNumber, amount);
            cout<<endl<<"Amount Withdrawn"<<endl;
        break;
        case 5:
            cout<<"Enter Account Number:";
            cin>>accountNumber;
            b.CloseAccount(accountNumber);
            cout<<endl<<"Account is Closed"<<endl;
        case 6:
            b.ShowAllAccounts();
        break;
        case 7:
            cout<<"\n\tSelect one option below ";
            cout<<"\n\t1 Personal Loan";
            cout<<"\n\t2 Home Loan";
            cout<<"\n\t3 Vehicle loan";
        case 0: 
            break;
        default:
            cout<<"\nEnter correct choice\n";
    }
 }while(choice!=0);
 return 0;
}