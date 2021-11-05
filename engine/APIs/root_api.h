#ifndef ROOT_API__H
#define ROOT_API__H

#include <root_factory.h>
#include <qeueu.h>

class RootAPI
{
  pArray<GroupID> m_rootNodes;

public:
  void init( pArray<GroupID> r )
  {
    m_rootNodes = r;
  }
  void startIteration( NodeChart * nodeArray, Qeueu & qeueu )
  {
    // first-grade gradient = 0 --> unsolvable
    // first_grade gradient = 1 --> solved state
    // initialize root nodes as solved with zero RotID --> first-grade gradient = 1
    qeueu.clean();
    for ( size_t id = 0; id < m_rootNodes -> size; ++ id )
    {
      const GroupID next =  m_rootNodes -> data[id];
      NodeChart & node = nodeArray[ next ];
      node.grade[0] = 1;
      node.aim  [0] = m_rootNodes -> value;
      qeueu << next;
    }
  }
};
#endif  //  ! ROOT_API__H