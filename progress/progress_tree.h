#ifndef ___PROGRESS_TREE__H
#define ___PROGRESS_TREE__H

#include <guide_handler.h>

template< cube_size N >
class ProgressTree: protected GuideHandler<N>
{
  using Guide = typename GuideFactory<N>::Guide;

  Rubik<N>  m_cube;

  Array<Node> m_path;

  Node * m_current;

  bool   m_transposed;
  CubeID m_transposition;

protected:

  static constexpr size_t TreeHeight = 11;

  ProgressTree();

  void setCube( const Rubik<N> & );
  void setCube( const Sequence & );
  void showCube() const;
  const Rubik<N> & getCube() const;

  void reset();
  bool setRoot();
  bool set( const size_t height );

  Node * root();
  const Node * root() const;

  void add( Guide, const ProgressTask );

  Sequence resolve( const Node * );
};

template< cube_size N >
ProgressTree<N>::ProgressTree()
  : m_path( TreeHeight )
{
  reset();
  m_current = m_path.begin();
}

template< cube_size N >
void ProgressTree<N>::setCube( const Rubik<N> & cube )
{
  m_cube = cube;
}

template< cube_size N >
void ProgressTree<N>::setCube( const Sequence & seq )
{
  m_cube.rotate( seq );
}

template< cube_size N >
void ProgressTree<N>::showCube() const
{
  m_cube.print();
}

template< cube_size N >
const Rubik<N> & ProgressTree<N>::getCube() const
{
  return m_cube;
}

template< cube_size N >
void ProgressTree<N>::reset()
{
  m_transposed = false;
  m_transposition = 0;
}

template< cube_size N >
bool ProgressTree<N>::set( const size_t height )
{
  for ( int d = 0; d <= height; ++ d )
  {
    m_path[d].depth = height - d;
  }
  return GuideHandler<N>::setRoot( m_path.begin(), &m_cube, m_transposition );
}

template< cube_size N >
Node * ProgressTree<N>::root()
{
  return m_path.begin();
}

template< cube_size N >
const Node * ProgressTree<N>::root() const
{
  return m_path.begin();
}

template< cube_size N>
void ProgressTree<N>::add( ProgressTree::Guide guide, const ProgressTask task )
{
  if ( ! m_transposed && _Scheduled == task )
  {
    m_transposition = guide.getTransposition( &m_cube );
    m_transposed  = true;
  }
  GuideHandler<N>::add( guide, task );
}

#endif  //  ! ___PROGRESS_TREE__H
