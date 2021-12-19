#include <sequence.h>

#include <text_output.h>
#include <fstream>

Sequence::Sequence()
  :Stack<RotID>( StackSize )
{
}

Sequence::Sequence( const size_t size)
  : Stack<RotID>( size )
{
}

Sequence::Sequence( const RotID* rotations, const size_t size )
  : Stack<RotID> ( StackSize )
{
  Array<RotID>::fill( rotations, rotations + size );
}

Sequence::Sequence( const RotID * beg, const RotID * end )
  : Stack<RotID>( StackSize )
{
  Array<RotID>::fill( beg, end );
}

Sequence & Sequence::operator << ( const RotID rotID )
{
  Stack<RotID>::push( rotID );
  return *this;
}

void Sequence::operator = (const Sequence & S )
{
  Stack<RotID>::reset();
  push( S.begin(), S.end() );
}

bool Sequence::operator == ( const Sequence & Seq ) const
{
  if ( size() != Seq.size() )
    return false;

  for ( auto P = begin(), S = Seq.begin(); P != end(); ++ P, ++ S )
  {
    if ( *P != *S )
    {
      return false;
    }
  }
  return true;
}

void Sequence::operator += ( const Sequence & S )
{
  Stack<RotID>::push( S.begin(), S.end() );
}

void Sequence::save( const std::string & fname, const size_t fsize ) const
{
  std::ofstream writeSeq( fname,std::ios::trunc );
  if ( writeSeq.is_open() )
  {
    const size_t end = fsize == 0 ? size() : fsize;
    for ( size_t step = 0; step < end; ++ step )
    {
      writeSeq << (int) Array<RotID>::get( step ) << ' ';
    }
    writeSeq.close();
  }
  else
  {
    clog( Color::red, Color::flash, "ERR:", fname, "could not be open for writing.", Color::off );
  }
}

void Sequence::load( const std::string & fname )
{
  reset();
  std::ifstream readSeq( fname );
  if ( readSeq.is_open() )
  {
    for( int next = 0;  readSeq >> next; )
    {
      push( next );
    }
    readSeq.close();
  }
  else
  {
    clog( Color::red, Color::flash, "ERR:", fname, "could not be open for writing.", Color::off );
  }
  
}
