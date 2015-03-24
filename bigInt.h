#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>
#include <vector>
#include <string>

const int BASE = 30000;   ////30000*30000<2^31 ,  so that Multiplication will never Overflow


class bigInt{
public:
	bigInt(){}
	bigInt(int num){     
		if(num>=0)
			m_positive=1;
		else{
			m_positive=0;
			num=0-num;
		}
		while(num!=0){
		m_value.push_back(num%m_base);
		num=num/m_base;
		}
	}
	bigInt(bigInt &num){
		this->m_positive=num.m_positive;
		for(size_t i=0;i<num.m_value.size();++i)
			this->m_value.push_back(num.m_value[i]);
	}
	~bigInt(){}

	bigInt bigAbs(){
		bigInt tmp(*this);
		if(this->m_positive==0)
			tmp.m_positive=1;
		return tmp;
	}

	bigInt operator +(bigInt &rhs){
		bigInt ret;
		if(this->m_positive&&rhs.m_positive){  //both positive
			std::vector<int>::iterator it1=this->m_value.begin();
			std::vector<int>::const_iterator it2=rhs.m_value.begin();
			int carry=0;
			unsigned int sum=0;
			while(it1!=this->m_value.end()&&it2!=rhs.m_value.end()){
				sum=(*it1)+(*it2)+carry;
				ret.m_value.push_back(sum%this->m_base);
				carry=sum/m_base;
				++it1;
				++it2;
			}
			while(it1!=this->m_value.end()){
				sum=(*it1)+carry;
				ret.m_value.push_back(sum%this->m_base);
				carry=sum/this->m_base;
				++it1;
			}
			while(it2!=rhs.m_value.end()){
				sum=(*it2)+carry;
				ret.m_value.push_back(sum%rhs.m_base);
				carry=sum/rhs.m_base;
				++it2;
			}
			if(carry>0)
				ret.m_value.push_back(carry);

			return ret;
		}
		else if(!this->m_positive&&rhs.m_positive){ //this:negtive  rhs:positive
			ret=rhs-((*this).bigAbs());
			return ret;
		}
		else if(this->m_positive&&!rhs.m_positive){  //this:positive  rhs:negtive
			ret = (*this)-rhs.bigAbs();
			return ret;
		}
		else{   //both negtive
			bigInt ret(((*this).bigAbs()+rhs.bigAbs()));
			ret.m_positive=0;
			return ret;
		}
	}	
	bigInt operator +=(bigInt &rhs){
		(*this)=((*this)+rhs);
		return (*this);
	}
	bigInt operator -(bigInt &rhs){		
		if((*this)==rhs){
			bigInt ret(0);
			return ret;
		}
		bigInt ret;
		std::vector<int>::iterator it1=this->m_value.begin();
		std::vector<int>::const_iterator it2=rhs.m_value.begin();
		if(this->m_positive&&rhs.m_positive){   //both positive
			if((*this)>rhs){
				ret.m_positive=1;
				int carry=0;
				unsigned int dif=0;
				while(it1!=this->m_value.end()&&it2!=rhs.m_value.end()){
					if((*it1)<(*it2)){
						*(it1+1)=*(it1+1)-1;
						*it1=*it1+m_base;
					}
					dif=(*it1)-(*it2);
					ret.m_value.push_back(dif);
					++it1;
					++it2;
				}
				while(it1!=this->m_value.end()){
					ret.m_value.push_back(*it1);
					++it1;
				}
				it1=ret.m_value.end()-1;
				while(it1!=ret.m_value.begin()){
					if(*it1==0){
						ret.m_value.erase(it1);
						it1=ret.m_value.end()-1;
					}
					else
						break;
				}
				return ret;
			}
			else{ /////(*this)<rhs
				ret=rhs-(*this);
				ret.m_positive=0;
				return ret;
			}
		}
		else if(!this->m_positive&&rhs.m_positive){  //this: negtive   rhs:positive
			return rhs+(this->bigAbs());			
		}
		else if(this->m_positive&&!rhs.m_positive){  //this: positive   rhs:negtive
			return rhs.bigAbs()+(*this);			
		}
		else{  //both negtive
			ret=rhs.bigAbs()-(this->bigAbs());
			return ret;
		}		
	}
	bigInt operator -=(bigInt &rhs){
		(*this)=((*this)-rhs);
		return (*this);
	}
	bigInt operator *(bigInt const &rhs){
		bigInt ret;
		if((this->m_positive+rhs.m_positive)%2==0) ///both positive or negtive
			ret.m_positive=1;
		else
			ret.m_positive=0;
		int carry[20]={0};
		bool used[20]={0};
		int buff[20]={0};
		int tmp=-1;
		int index=-1;
		for(size_t i=0;i<m_value.size();++i){
			for(size_t j=0;j<rhs.m_value.size();++j){
				unsigned int pduct=m_value[i]*(rhs.m_value[j]);
				if(used[i+j]){
					buff[i+j]+=(pduct)%m_base;
					carry[i+j+1]=pduct/m_base;
				}
				else{
					buff[i+j]+=(pduct+carry[i+j])%m_base;
					carry[i+j+1]=(pduct+carry[i+j])/m_base;
					used[i+j]=1;
				}
				tmp=carry[i+j+1];
				index=i+j;
				
			}
		}
		for(int i=0;i<=index;++i){
				ret.m_value.push_back(buff[i]);
		}
		if(tmp>0)			
			ret.m_value.push_back(tmp);
		return ret;
	}
	bigInt& operator *=(bigInt const &rhs){
		(*this)=((*this)*rhs);
		return (*this);
	}
	bigInt operator *(int const &rhs){
		bigInt ret(rhs);
		return (*this)*ret;
	}
	bigInt operator *=(int const &rhs){
		bigInt ret(rhs);
		(*this)=((*this)*rhs);
		return (*this);
	}

	/*	compare functions	*/
	int compare(bigInt const &rhs){
		if((!this->m_positive)&&(rhs.m_positive))//this:negtive  rhs:positive
			return -1;
		else if((this->m_positive)&&(!rhs.m_positive))//this:positive  rhs:negtive
			return 1;
		int flag=1;
		if((!this->m_positive)&&(!rhs.m_positive))
			flag=-1;
		if(this->m_value.size()>rhs.m_value.size())
			return 1*flag;
		else if(this->m_value.size()<rhs.m_value.size())
			return -1*flag;
		else{
			for(int i=this->m_value.size()-1;i>=0;--i)
			{
				if(this->m_value[i]>rhs.m_value[i])
					return 1*flag;
				else if(this->m_value[i]<rhs.m_value[i])
					return -1*flag;
			}
			return 0;////0:equal 1:this>rhs  -1:this<rhs
		}
	}	
	bool operator >(bigInt const &rhs){
		return compare(rhs)==1;
	}
	bool operator >=(bigInt const &rhs){
		return compare(rhs)>=0;
	}
	bool operator <(bigInt const &rhs){
		return compare(rhs)==-1;
	}
	bool operator <=(bigInt const &rhs){
		return compare(rhs)<=0;
	}
	bool operator ==(bigInt const &rhs){
		return compare(rhs)==0;
	}
	bool operator !=(bigInt const &rhs){
		return !compare(rhs)==0;
	}

	/*    test  helper functions  */
	bool getFlag() const{
		return m_positive;
	}
	int getValueSize() const{
		return m_value.size();
	}
	int getBase(){
		return m_base;
	}

private:
	std::vector<int> m_value;
	bool m_positive;////false:0  true:1
	static const int m_base=BASE;
};

#endif