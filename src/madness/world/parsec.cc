#include "parsec.h"
#include <iostream>
#include "thread.h"
#include <dague/dague_internal.h>

// Here we initialize with the right child class
#if HAVE_PARSEC
namespace madness {
    dague_hook_return_t release_madness_task (dague_execution_unit_t *eu, 
                                              dague_execution_context_t *exec_context)
    {
        char* s;
        PoolTaskInterface *c;
        s=(char*)exec_context;
        s-=offsetof(PoolTaskInterface, exec_context);
        c = (PoolTaskInterface*)s;
        delete(c);
        return DAGUE_HOOK_RETURN_DONE;
    }
    
    dague_hook_return_t complete_madness_task (dague_execution_unit_t *eu, 
                                               dague_execution_context_t *exec_context)
    {
        dague_atomic_add_32b(&exec_context->dague_handle->nb_tasks, -1);
        return DAGUE_HOOK_RETURN_DONE;
    }

// void c_run (void* s) {
//   TaskInterface *c = (TaskInterface *)s;
//   return c->run();
// }

// int c_sum2 (void* s, void *p) {
//   Integer *c = (Integer *)s;
//   Integer *d = (Integer *)p;
//   return c->sum(d);
// }

const dague_function_t* PoolTaskInterface::func = &madness_function;

dague_hook_return_t pointer_call(dague_execution_unit_t *eu, 
				 dague_execution_context_t *exec_context)
{
  char* s;
  PoolTaskInterface *c;
  s=(char*)exec_context;
  s-=offsetof(PoolTaskInterface, exec_context);
  c = (PoolTaskInterface*)s;
  std::cout << "in pointer call " << std::endl;
  c->run(TaskThreadEnv(1, 0, 0));
  return DAGUE_HOOK_RETURN_DONE;
}


  //};

// void Task::run(void)
// {
//   //return Integer::func.c + this->exec_context.a;
//   std::cout << "it works !!!!! " << "\n";
// }

// dague_execution_context_t* c_madness_context(void *s)
// {
//   TaskInterface *c = (TaskInterface *)s;
//   std::cout << "in c_madness_context" << std::endl;
//   return c->madness_context(c);
// }

// dague_execution_context_t* Task::madness_context(TaskInterface *c)
// {
//   //  TaskInterface *c = (TaskInterface *)s;
//   std::cout << "in madness_context" << std::endl;
//   return &(c->exec_context);
// }


extern "C"{

#include <stdio.h>

static uint64_t RUN_hash(const dague_handle_t *handle, const assignment_t *a)
{
  (void)a; (void)handle;
  return 0;
}

static dague_hook_return_t empty_hook (dague_execution_unit_t *eu, 
                          dague_execution_context_t *context)
{
  return DAGUE_HOOK_RETURN_DONE;
}

static const __dague_chore_t __RUN_chores[] = {
    {.type = DAGUE_DEV_CPU,
	.evaluate = NULL,
    .hook = pointer_call},
    {.type = DAGUE_DEV_NONE,
	.evaluate = NULL,
    .hook = (dague_hook_t *) NULL},
};


const dague_function_t madness_function = {
    .name = "RUN",
    .flags = DAGUE_HAS_IN_IN_DEPENDENCIES | DAGUE_USE_DEPS_MASK,
    .function_id = 0,
    .nb_flows = 0,
    .nb_parameters = 0,
    .nb_locals = 0,
    .dependencies_goal = 0,
    .params = {NULL,NULL,NULL, NULL},
    .locals = {NULL,NULL,NULL, NULL},
    .in = {NULL, NULL, NULL, NULL},
    .out = {NULL, NULL, NULL, NULL},
    .priority = NULL,
    .initial_data = NULL,
    .final_data = NULL,
    .data_affinity = NULL,
    .key = (dague_functionkey_fn_t *) RUN_hash,
#if defined(DAGUE_SIM)
    .sim_cost_fct = (dague_sim_cost_fct_t*) NULL,
#endif
    .get_datatype = (dague_datatype_lookup_t *) NULL,
    .prepare_input = empty_hook,
    .incarnations = __RUN_chores,
    .prepare_output = (dague_hook_t *) NULL,
    .find_deps = (dague_find_dependency_fn_t*)NULL,
    .iterate_successors = (dague_traverse_function_t *) NULL,
    .iterate_predecessors = (dague_traverse_function_t *) NULL,
    .release_deps = (dague_release_deps_t *) NULL,
    .complete_execution = complete_madness_task, // complete MADNESS task and generate successors
    .new_task = (dague_new_task_function_t*) NULL,
    .release_task = release_madness_task, //object delete,
    .fini = (dague_hook_t *) NULL,
};

  const dague_function_t* madness_function_array[]= {&(madness::madness_function), NULL};

dague_handle_t madness_handle {
  .super = { 0x0, },
  .handle_id = 0,
  .nb_tasks = 0,
  .nb_functions = 1,
  .devices_mask = DAGUE_DEVICES_ALL,
  .priority = 0,
  .nb_pending_actions = 1,
  .context = NULL,
  .startup_hook = NULL,
  .functions_array = madness_function_array,
};
}
}
#endif
