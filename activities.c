#include "headers.h"
int cmp (const void* a, const void* b)
{
    info **entryA = (info **)a;
    info **entryB = (info **)b;
    return ((*entryA)->pid- (*entryB)->pid);
}
void activities ()
{
    qsort (act_list, num_act_list, sizeof (info*), cmp);
    for (int i=0; i<num_act_list; i++)
    {
        if (strcmp (act_list[i]->state, "Terminated")==0 || strcmp (act_list[i]->state, "Killed")==0)
        continue;
        else
        {
            printf ("%d : %s - %s\n", act_list[i]->pid, act_list[i]->command, act_list[i]->state);
        }
    }
    return;
}