#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkage long sys_hello(void) {
	printk("Hello, World!\n");
	return 0;
}

asmlinkage long sys_set_weight(int weight) {
	if(weight<0)
		return (long)-EINVAL;
	current->weight = weight;
	return 0;
}

asmlinkage long sys_get_weight(void) {
	return current->weight;
}


long get_leaf_aux(struct task_struct *head) {
	struct list_head *child_h;
	struct task_struct *ts_ptr;
	long sum = 0;
	
	list_for_each(child_h, &head->children){
		ts_ptr = list_entry(child_h, struct task_struct, sibling);
		if(list_empty(&ts_ptr->children)){
			sum += ts_ptr->weight;
		}
		sum += get_leaf_aux(ts_ptr);
	}
	return sum;
}

asmlinkage long sys_get_leaf_children_sum(void) {

	if(list_empty(&current->children)) {
		return -ECHILD;
	}
	return get_leaf_aux(current);	
}

/*void get_heaviest_ancestor_aux(struct task_struct *current, pid_t** pid_h, int** weight_h) {
	if(current == NULL)
		return;
	if(current->weight_hw2 > **weight_h) {
		**weight_h = current->weight_hw2;
		**pid_h = current->pid;
	}
	struct task_struct *p = current->parent;
	get_heaviest_ancestor_aux(p, &pid_h, &weight_h);
	
}*/
asmlinkage long sys_get_heaviest_ancestor(void) {
	long maxW = 0;
	long maxPID = 1;
	struct task_struct* parentT = current;
	while(parentT-> pid != 1){
		if(parentT->weight > maxW){
			maxW = parentT->weight;
			maxPID = parentT->pid;
		}
		parentT = parentT->parent;
	}
	
	return parentT->weight > maxW ? 1 : maxPID;
}
