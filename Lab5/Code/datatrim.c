/*
The script to fetch the subset of the original data and convert it to a good shape. Index start from 0.
As for the output file, every line represents a directed link with the first number as the index of the source node and the second number as the index of the destination node.

-----
Compiling:
    > gcc datatrim.c -o datatrim

-----
Synopsis:
    datatrim [-bion]

-----
Options:
    -b    specify the upper bound index to be included in the original data (default 5300, generating data with 1112 nodes)
    -i    specify the input path (default "./web-Stanford.txt")
    -o    specify the output path (default "./data_input") 
    -n    tag to open the auto link addition for the nodes that have no out going links

-----
Outputs:
    Output file:
    data_input:     Output file in which every line indicates a directed link with the first number as the index of the source node and the second number as the index of the destination node.

-----
Error returns:
    -1    unexpected options
    -2    fail to open files 

-----
Example:
    >datatrim
    fetch the graph from the original and store the result in "./data_input", 
   
    >datagen -s 10000 -n
    fetch the graph with index less than 10000 and store it in "data_input"

Source data is from:
http://snap.stanford.edu/data/web-Stanford.html
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char* argv[]){
    int option;
    int b_extend = 0;
    char *INPATH = "web-Stanford.txt";
    char *OUTPATH = "data_input";
    int BOUND = 5300, src, dst;
    FILE *fp_ori, *fp_dest;
    int *flag, *num_out_links;
    int Ecount = 0;
    int Ncount = 0;
    int Noncount = 0;
    int i,j;
    char* tempstore;

    while ((option = getopt(argc, argv, "b:i:o:n")) != -1)
        switch(option){
            case 'b': BOUND = strtol(optarg, NULL, 10); break;
            case 'i': INPATH = optarg; break;
            case 'o': OUTPATH = optarg; break;
            case 'n': b_extend = 1; break;
            case '?': return -1;
        }
    if ((fp_ori = fopen(INPATH,"r")) == NULL){
        printf("Fail to open the source data file. \n");
        return -2;
    }  
    if ((fp_dest = fopen(OUTPATH,"w")) == NULL){
        printf("Fail to open the output file. \n");
        return -2;
    }
    flag = malloc(BOUND*sizeof(int));
    tempstore = malloc(100*sizeof(char));

    // count the nodes and map
    for (i = 0; i < BOUND; ++i)
        flag[i] = 0;     
    for (j = 0; j < 4; ++j){
        fgets(tempstore, 100, fp_ori);
    }
    while(!feof(fp_ori)){
        fscanf(fp_ori, "%d\t%d\n", &src, &dst);
        if (src < BOUND && dst < BOUND){
            ++Ecount;
            flag[src] = 1;
            flag[dst] = 1;
        }
    }
    for (i = 0; i < BOUND; ++i)
        Ncount+=flag[i];
    j=0;
    if (BOUND == 1)
        --flag[0];
    else{
        for (i = 0; i< BOUND; ++i)
            if (flag[i]){
                flag[i] = j++;
            }else{
                flag[i] = -1; //tag to indicate that the current edge is not engaged.
            }
    }
    rewind(fp_ori);   
    for (j = 0; j < 4; ++j){
        fgets(tempstore, 100, fp_ori);
    }
    free(tempstore);
    //fprintf(fp_dest, "%d\n", Ncount);
    if (b_extend){ 
        num_out_links = malloc(Ncount * sizeof(int));
        for (i = 0; i < Ncount; ++i) num_out_links[i] = 0;
    }
    while(!feof(fp_ori)){
        fscanf(fp_ori, "%d\t%d\n", &src, &dst);
        if (src < BOUND && dst < BOUND){
            fprintf(fp_dest, "%d\t%d\n", flag[src], flag[dst]);
            if (b_extend) ++num_out_links[flag[src]];
        }
    }
    if (b_extend){
        for (i = 0; i < Ncount; ++i)
            if (num_out_links[i] == 0){
                ++Noncount;
                for (j = 0; j < BOUND; ++j)
                    if (flag[j] >= 0)
                        fprintf(fp_dest, "%d\t%d\n", i, flag[j]);
            }
        free(num_out_links);   
    }
    if (b_extend){
        Ecount += Ncount * Noncount;
        printf("There are %d nodes and %d edges in the sub dataset including the added edges for the nodes with no outgoing links. \n", Ncount, Ecount);
    }
    else
        printf("There are %d nodes and %d edges in the sub dataset. \n", Ncount, Ecount);

    free(flag);
    fclose(fp_ori);
    fclose(fp_dest);
    return 0;
}
