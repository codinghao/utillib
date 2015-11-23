#ifndef _RBTREE_H_
#define _RBTREE_H_

class rbtree
{
public:
    enum
    {
        RB_RED,
        RB_BLACK,
    };

    struct rb_node
    {
        rb_node(int data)
            : rb_color_(RB_RED)
            , rb_left_(NULL)
            , rb_right_(NULL)
            , rb_parent_(NULL)
            , rb_data_(data)
        {}

        char  rb_color_;
        rb_node* rb_left_;
        rb_node* rb_right_;
        rb_node* rb_parent_;
        int   rb_data_;
    };

    rbtree()
        : root_(NULL)
    {}

    void insert(int data)
    {
        rb_node *node = new rb_node(data);
        if (root_ == NULL)
        {
            root_ = node;
            return ;
        }

        rb_node* tmp = rb_root_;
        while(tmp != NULL)
        {
            if (data > tmp->rb_data_)
            {
                if (tmp->rb_right_ == NULL)
                {
                    tmp->rb_right = node;
                    node->rb_parent_ = tmp;
                    insert(node);
                    break;
                }
                else
                {
                    tmp = tmp->rb_right_;
                    continue;
                }
            }
            else
            {
                if (tmp->rb_left_ == NULL)
                {
                    tmp->rb_left_ = node;
                    node->rb_parent_ = tmp;
                    insert(node);
                    break;
                }
                else
                {
                    tmp = tmp->rb_left_;
                    continue;
                }
            }
        }
    }

    void insert(rb_node* node)
    {
        rb_node* parent = node->rb_parent_;
        if (parent->rb_color_ == RB_BLACK)
            return ;

        rb_node* gparent = parent->rb_parent_;

    }

    void debug()
    {
        
    }

private:
    rb_node* rb_root_;
};

#endif
