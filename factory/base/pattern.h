#ifndef ___CUBE_PATTERN_H
#define ___CUBE_PATTERN_H

#include <cube_set.h>
#include <algorithm>
#include <rubik.h>

template< cube_size N >
class Pattern: public Stack<PosID>
{
  BitMapID  m_priorRotIDs;
  PosID     m_priorPos;

  void setPrior()
  {
    m_priorPos = top();
    m_priorRotIDs = CPositions<N>::ActOn( m_priorPos );
  }

public:
  Pattern() = default;
  Pattern( const size_t );
  Pattern( const Pattern<N> & ) = default;
  Pattern( const std::initializer_list<PosID> & list );
  Pattern( const size_t size, const PosID * ref );

  PosID getPriorPos() const
  {
    return m_priorPos;
  }

  BitMapID priorRotIDs() const
  {
    return m_priorRotIDs;
  }

  BitMapID priorRotIDs( const CubeID prior ) const
  {
    return CubeSet::GetCubeSet( prior, m_priorRotIDs );
  }

  bool priorMoving( const RotID rotID ) const
  {
    return m_priorRotIDs & ( 1ULL << rotID );
  }

  bool priorMoving( const CubeID prior, const RotID rotID ) const
  {
    return m_priorRotIDs & ( 1ULL << ( CRotations<N>::GetRotID( rotID, Simplex::Inverse( prior ) ) ) );
  }

  CubeID movePrior( const CubeID prior, const RotID rotID ) const;
  bool valid( GroupID ) const;

  CubeID getPrior ( const Rubik<N> * Cube, const CubeID trans = 0 ) const;
  GroupID getState( const Rubik<N> * Cube, const CubeID trans = 0 ) const;

  Pattern<N> transpose  ( const CubeID ) const;
  Pattern<N> operator * ( const CubeID ) const;
  bool operator == ( const Pattern<N> & ) const;
  bool getTransposition( const Pattern<N> & from, CubeID & trans /*result*/ ) const;
  void print() const;
};

template< cube_size N > Pattern<N>::Pattern( const size_t size )
  : Stack<PosID>( size )
{
}

template< cube_size N >
Pattern<N>::Pattern( const size_t size, const PosID * ref )
  : Stack<PosID>( size )
{
  init( size, ref );
  std::sort( begin(), end() );
  setPrior();
}

template< cube_size N >
Pattern<N>::Pattern( const std::initializer_list<PosID> & list )
  : Stack<PosID>( list.size() )
{
  push( list.begin(), list.end() );
  std::sort( begin(), end() );
  setPrior();
}

template< cube_size N >
CubeID Pattern<N>::movePrior( const CubeID prior, const RotID rotID ) const
{
  return priorMoving( prior, rotID ) ? CRotations<N>::Tilt( prior, rotID ) : prior;
}

template< cube_size N >
bool Pattern<N>::valid( GroupID gid ) const
{
  BoolArray positions( CPositions<N>::GetSize() );

  for ( size_t index = 0; index < size(); ++ index, gid /= 24 )
  {
    const PosID next = CPositions<N>::GetPosID( m_array[ index ], gid % 24 );
    if ( positions[ next ] )
      return false;
    positions.set( next, true );
  }
  return true;
}

template< cube_size N >
CubeID Pattern<N>::getPrior(const Rubik<N>* Cube, const CubeID trans ) const
{
  return Cube -> transpose( getPriorPos(), trans );
}

template< cube_size N >
GroupID Pattern<N>::getState( const Rubik<N> * Cube, const CubeID trans ) const
{
  BitMapID result = 0;
  const CubeID invPrior = Simplex::Inverse( getPrior( Cube, trans ) );
  for ( int id = 0; id < size() - 1; ++ id )
  {
    result += Simplex::Composition( Cube -> transpose( m_array[id], trans ), invPrior ) * pow24( id );
  }
  return result;
}

template< cube_size N >
Pattern<N> Pattern<N>::transpose( const CubeID transposition ) const
{
  Pattern<N> result( size() );

  size_t index = 0;
  for ( auto P : *this )
  {
    result.push( CPositions<N>::GetPosID( P, transposition ) );
  }
  std::sort( result.begin(), result.end() );
  result.setPrior();
  return result;
}

template< cube_size N >
Pattern<N> Pattern<N>::operator * ( const CubeID transposition ) const
{
  return transpose( transposition );
}

template< cube_size N >
bool Pattern<N>::operator == ( const Pattern<N> & p ) const
{
  for ( size_t id = 0; id < size(); ++ id )
  {
    if ( get( id ) != p[id] )
    {
      return false;
    }
  }
  return true;
}

template< cube_size N >
bool Pattern<N>::getTransposition( const Pattern<N> & from, CubeID & trans ) const
{
  all_cubeid( cid )
  {
    if ( from * cid == *this )
    {
      trans = cid;
      return true;
    }
  }
  return false;
}

template< cube_size N >
void Pattern<N>::print() const
{
  clog( "size:", size() );
  for ( auto P : *this )
  {
    CPositions<N>::Print( P );
  }
}

#endif  //  ! ___CUBE_PATTERN_H
