#ifndef __MYCOMPAREDOC_H__
#define __MYCOMPAREDOC_H__
#include <vector>
#include <utility>
#include <math.h>
class MyCompareDoc 
{
	public:
		MyCompareDoc( std::vector<double> & weight_vec): m_vec(weight_vec)
		{
	
		}
		bool operator()(const std::pair<int, std::vector<double> >& left, const std::pair<int, std::vector<double> >& right )
		{
			double weight_left = 0 ;
			double weight_right = 0 ;
			double len_left = 0 ;
			double len_right = 0 ;
			int index ;
			for(index = 0 ; index != m_vec.size(); index ++)
			{
				weight_left += (left.second)[index] * m_vec[index] ;
				weight_right += (right.second)[index] * m_vec[index] ;
				len_left += (left.second)[index] * (left.second)[index] ;
				len_right += (right.second)[index] * (right.second)[index] ;
			}
			if(weight_left/sqrt(len_left) < weight_right / sqrt(len_right))
			{
				return false ;
			}else 
			{
				return true ;
			}
		}
	private:
		std::vector<double> &m_vec ;
};
#endif
