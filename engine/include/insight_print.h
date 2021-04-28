#ifndef INSIGHT_PRINT_H_INCLUDED
#define INSIGHT_PRINT_H_INCLUDED

#include <insight.h>

static constexpr Orient _side[ 3 ][ 4 ] = { 
      { _NF,  _U, _NF, _NF },
      {  _L,  _F,  _R,  _B },
      { _NF,  _D, _NF, _NF }
 };

template< cube_size N >
static bool GetCoords( Layer & xCoord, Layer & yCoord, const Orient side, const PosID place )
{
  switch( side )
  {
  case _U:
    if ( CPositions<N>::GetLayer( place, _Y ) < N - 1 )
      return false;
    xCoord = CPositions<N>::GetLayer( place, _X );
    yCoord = CPositions<N>::GetLayer( place, _Z );
    break;

  case _L:
    if ( CPositions<N>::GetLayer( place, _X ) > 0 )
      return false;
    xCoord =         CPositions<N>::GetLayer( place, _Z );
    yCoord = N - 1 - CPositions<N>::GetLayer( place, _Y );
    break;
    
  case _F:
    if ( CPositions<N>::GetLayer( place, _Z ) < N - 1 )
      return false;
    xCoord =         CPositions<N>::GetLayer( place, _X );
    yCoord = N - 1 - CPositions<N>::GetLayer( place, _Y );
    break;
      
  case _R:
    if ( CPositions<N>::GetLayer( place, _X ) < N - 1 )
      return false;
    xCoord = N - 1 - CPositions<N>::GetLayer( place, _Z );
    yCoord = N - 1 - CPositions<N>::GetLayer( place, _Y );
    break;

  case _B:
    if ( CPositions<N>::GetLayer( place, _Z ) > 0 )
      return false;
    xCoord = N - 1 - CPositions<N>::GetLayer( place, _X );
    yCoord = N - 1 - CPositions<N>::GetLayer( place, _Y );
    break;
  
  case _D:
    if ( CPositions<N>::GetLayer( place, _Y ) > 0 )
      return false;
    xCoord =         CPositions<N>::GetLayer( place, _X );
    yCoord = N - 1 - CPositions<N>::GetLayer( place, _Z );
    break;

  default:
    return false;
  }
  return true;
}

template< cube_size N >
void Insight<N>::print( const bool details ) const
 {
   PosID * pos = new PosID [ m_size ];
   SetCacheID( pos, m_stateID, m_size, m_prior );

   for( size_t m = 0; details && m < m_size; ++ m )
   {
     clog( CPositions<N>::GetCoord( m_pos[m] ).toString(), "-->",  CPositions<N>::GetCoord( CPositions<N>::GetPosID( m_pos[m], pos[m] ) ).toString() );
   }

   for( size_t raw = 0; raw < 3 * N; ++ raw )
   {
     if ( raw % N == 0 )
       NL();
     
     const size_t colMax = ( N <= raw && raw < 2 * N ) ? 4 * N : 2 * N;
     for( size_t col = 0; col < colMax; ++ col )
     {
       if ( col % N == 0 )
         clog_ ( ' ' );

       const Orient side = _side[ raw / N ][ col / N ];
       if ( side == _NF )
       {
         clog_( ' ' );
         continue;
       }
       size_t pid = 0;
       while (  pid < m_size )
       {
          Layer xCoord;
          Layer yCoord;
          
          const PosID place = CPositions<N>::GetPosID( m_pos[pid], pos[pid] );
          
          if ( GetCoords <N> ( xCoord, yCoord, side, place ) && xCoord == col % N && yCoord == raw % N )
          {
            clog_( colorOf( Simplex::GetCube( pos[pid] ).whatIs( side ) ), FChar, Color::off );
            break;
          }
          ++ pid;
       }
       if ( pid == m_size )
       {
         clog_( Color::dark, FChar, Color::off );
       }
     }
     NL();
   }
   delete[] pos;
 }

#endif // INSIGHT_PRINT_H_INCLUDED
