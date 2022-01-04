#ifndef ___CUBE_PATTERN_H
#define ___CUBE_PATTERN_H

#include <bool_array.h>
#include <cube_set.h>
#include <dynamic_stack.h>
#include <algorithm>
#include <cube_rotations.h>
#include <cube_positions.h>

template< cube_size N >
class Pattern: public Stack<PosID>
{
  BitMapID  m_priorRotIDs;
  PosID     m_priorPos;

  void setPrior()
  {
    std::sort( begin(), end() );
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

  Pattern<N> transpose  ( const CubeID ) const;
  Pattern<N> operator * ( const CubeID ) const;
  bool operator == ( const Pattern<N> & ) const;
  bool isTransposed( const Pattern<N> & from, CubeID & trans /*result*/ ) const;
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
  setPrior();
}

template< cube_size N >
Pattern<N>::Pattern( const std::initializer_list<PosID> & list )
  : Stack<PosID>( list.size() )
{
  push( list.begin(), list.end() );
  setPrior();
}

template< cube_size N > inline
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
    {
      return false;
    }
    positions.set( next, true );
  }
  return true;
}

template< cube_size N > inline
Pattern<N> Pattern<N>::transpose( const CubeID transposition ) const
{
  Pattern<N> result( size() );

  for ( auto P : *this )
  {
    result.push( CPositions<N>::GetPosID( P, transposition ) );
  }
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
bool Pattern<N>::isTransposed( const Pattern<N> & from, CubeID & trans ) const
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
    clog_( CPositions<N>::ToString( P ) );
  }
}

#endif  //  ! ___CUBE_PATTERN_H
