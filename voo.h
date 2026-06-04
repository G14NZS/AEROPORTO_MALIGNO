#ifndef VOO_H
#define VOO_H

#include "aeroporto.h"
#include "matriz_esparsa.h"

Status cadastrarVoo(VetorAeroportos *aeroportos,
                    MatrizEsparsa *malha,
                    const char *codigoOrigem,
                    const char *codigoDestino,
                    int numeroVoo);

Status listarVoosSaindoDe(VetorAeroportos *aeroportos,
                          MatrizEsparsa *malha,
                          const char *codigoOrigem);

Status removerVoo(VetorAeroportos *aeroportos,
                  MatrizEsparsa *malha,
                  const char *codigoOrigem,
                  const char *codigoDestino);
#endif
