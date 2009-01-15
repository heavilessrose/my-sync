#ifndef GRAPHADT_H
  #include "graphadt.h"
#endif
#ifndef NETWORKADT_H
  #include "networkadt.h"
#endif
#ifndef STACKADT_H
  #include "stackadt.h"
#endif

int main()
{
  GraphAdt<long> Lgraph_(1, 20);
  NetworkAdt<long, long> Lnetwork_(1, 20);
  StackAdt<long> Lstack_;

  return 0;
}


