#include <stdio.h>
#include <stdlib.h>

#define NUMBER_ORGANISMS 100
#define NUMBER_GENES     20
#define ALLELES          4
#define MUTATION_RATE    0.001

#define MAXIMUM_FITNESS NUMBER_GENES
#define FALSE 0
#define TRUE  1


char **currentGeneration, **nextGeneration;
char *modelOrganism;
int *organismsFitnesses;
int totalOfFitnesses;


void AllocateMemory(void);
int DoOneRun(void);
void InitializeOrganisms(void);
int EvaluateOrganisms(void);
void ProduceNextGeneration(void);
int SelectOneOrganism(void);


int main(){
  int finalGeneration;
  AllocateMemory();
  finalGeneration = DoOneRun();
  printf("The final generation was: %d\n",
         finalGeneration);
}


void AllocateMemory(void){
  int organism;

  currentGeneration =
    (char**)malloc(sizeof(char*) * NUMBER_ORGANISMS);
  nextGeneration =
    (char**)malloc(sizeof(char*) * NUMBER_ORGANISMS);
  modelOrganism =
    (char*)malloc(sizeof(char) * NUMBER_GENES);
  organismsFitnesses =
    (int*)malloc(sizeof(int) * NUMBER_ORGANISMS);

  for(organism=0; organism<NUMBER_ORGANISMS; ++organism){
    currentGeneration[organism] =
      (char*)malloc(sizeof(char) * NUMBER_GENES);
    nextGeneration[organism] =
      (char*)malloc(sizeof(char) * NUMBER_GENES);
  }
}


int DoOneRun(void){
  int generations = 1;
  int perfectGeneration = FALSE;

  InitializeOrganisms();

  while(TRUE){
    perfectGeneration = EvaluateOrganisms();
    if( perfectGeneration==TRUE ) return generations;
    ProduceNextGeneration();
    ++generations;
  }
}


void InitializeOrganisms(void){
  int organism;
  int gene;

  // inicia o organismo normal
  for(organism=0; organism<NUMBER_ORGANISMS; ++organism){
    for(gene=0; gene<NUMBER_GENES; ++gene){
      currentGeneration[organism][gene] = rand()%ALLELES;
    }
  }

  // inicia o modelo do organismo
  for(gene=0; gene<NUMBER_GENES; ++gene){
    modelOrganism[gene] = rand()%ALLELES;
  }
}


int EvaluateOrganisms(void){
  int organism;
  int gene;
  int currentOrganismsFitnessTally;

  totalOfFitnesses = 0;

  for(organism=0; organism<NUMBER_ORGANISMS; ++organism){
    currentOrganismsFitnessTally = 0;

    // conta a aptidão dos organismos
    for(gene=0; gene<NUMBER_GENES; ++gene){
      if( currentGeneration[organism][gene]
          == modelOrganism[gene] ){
        ++currentOrganismsFitnessTally;
      }
    }

    //salvar e adicionar sua aptidão ao total da geração
    organismsFitnesses[organism] =
      currentOrganismsFitnessTally;
    totalOfFitnesses += currentOrganismsFitnessTally;

    // verifica se há geracao perfeita
    if( currentOrganismsFitnessTally == MAXIMUM_FITNESS ){
      return TRUE;
    }
  }
  return FALSE;
}


void ProduceNextGeneration(void){
  int organism;
  int gene;
  int parentOne;
  int parentTwo;
  int crossoverPoint;
  int mutateThisGene;

  //preenchimento da proxima geração
  for(organism=0; organism<NUMBER_ORGANISMS; ++organism){
    parentOne = SelectOneOrganism();
    parentTwo = SelectOneOrganism();
    crossoverPoint  =  rand() % NUMBER_GENES;

    for(gene=0; gene<NUMBER_GENES; ++gene){

      // copiar um gene
      mutateThisGene = rand() % (int)(1.0 / MUTATION_RATE);
      if(mutateThisGene == 0){

        // fazer desse gene um mutação
        nextGeneration[organism][gene] = rand() % ALLELES;
      } else {
        // copiar esse gene de um pai
        if (gene < crossoverPoint){
          nextGeneration[organism][gene] =
            currentGeneration[parentOne][gene];
        } else {
          nextGeneration[organism][gene] =
            currentGeneration[parentTwo][gene];
        }
      }
    }
  }

  // copiar a proxima geração para a geração atual
  for(organism=0; organism<NUMBER_ORGANISMS; ++organism){
    for(gene=0; gene<NUMBER_GENES; ++gene){
      currentGeneration[organism][gene] =
        nextGeneration[organism][gene];
    }
  }
}


int SelectOneOrganism(void){
  int organism;
  int runningTotal;
  int randomSelectPoint;

  runningTotal = 0;
  randomSelectPoint = rand() % (totalOfFitnesses + 1);

  for(organism=0; organism<NUMBER_ORGANISMS; ++organism){
    runningTotal += organismsFitnesses[organism];
    if(runningTotal >= randomSelectPoint) return organism;
  }
}
