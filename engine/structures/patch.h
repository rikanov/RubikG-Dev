#ifndef PATCH__H
#define PATCH__H

struct Patch
{
  const size_t   size;
  const PosID  * refPosition;

  Patch( const PosID *, const size_t );
};

Patch::Patch( const PosID * ref, const size_t s )
  : size( s )
  , refPosition( ref )
{

}




#endif  //  ! PATCH__H
