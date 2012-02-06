#ifndef GHMM_GHMM_H_
#define GHMM_GHMM_H_


#include "ghmm_default_traits.hpp"
#include <boost/graph/copy.hpp>


namespace ghmm
{


template < typename T, int N, int FULL_N,  typename GHMM_TRAITS = GHMMDefaultTraits< T, N, FULL_N > >
class GHMM
{
public:
  typedef GHMM_TRAITS traits_type;
  typedef typename GHMM_TRAITS::full_observation_type full_observation_type;
  typedef typename GHMM_TRAITS::observation_type observation_type;
  typedef typename GHMM_TRAITS::value_type value_type;
  typedef typename GHMM_TRAITS::graph_type graph_type;
  typedef typename GHMM_TRAITS::distance_type distance_type;
  typedef typename GHMM_TRAITS::gaussian_type gaussian_type;
  typedef typename GHMM_TRAITS::full_matrix_type full_matrix_type;
  typedef typename GHMM_TRAITS::matrix_type matrix_type;
  typedef typename GHMM_TRAITS::itm_type itm_type;
  typedef typename GHMM_TRAITS::itm_type::node_type node_type;
  typedef typename itm_type::node_iterator node_iterator;
  typedef typename itm_type::out_edge_iterator out_edge_iterator;
  typedef typename itm_type::in_edge_iterator in_edge_iterator;
  typedef typename GHMM_TRAITS::estimations_type estimations_type;

  GHMM( 
    full_matrix_type fullSigma, 
    matrix_type sigma, 
    value_type insertionDistance, 
    value_type epsilon,
    value_type statePrior,
    value_type transitionPrior
  );

  void initTrack( graph_type & graph ) const;
  void update( graph_type & graph, const observation_type & o ) const;
  void predict( graph_type & graph, uint8_t horizon ) const;
  value_type observationPdf( 
    const graph_type & graph, 
    uint32_t t, 
    const observation_type & o 
  ) const;

  template < typename IT >
  void learn( IT begin, IT end );

  graph_type & graph();
private:
  typedef typename std::vector< value_type > value_array;
  matrix_type   sigma_;
  value_type    statePrior_;
  value_type    transitionPrior_;
  graph_type    graph_;
  itm_type      itm_;
  gaussian_type gaussian_;
  uint32_t      trajectoryCount_;

  value_array factors_;

  void normalize();

  template < typename IT >
  void computeForward( IT begin, IT end );

  template < typename IT >
  void computeBackwards( IT begin, IT end );

  template < typename IT >
  void updateParameters( IT begin, IT end );

  value_type observationProbability( 
    const observation_type & o, 
    const node_type & n 
  ) const;
};


#include "GHMM-inline.hpp"


}


#endif //GHMM_GHMM_H_

