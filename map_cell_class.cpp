#include "map_cell_class.h"

const std::vector<QString> all_tile_options = {"deep", "shallow","beach","grass","forest","mountain"};
const QString path = "C:\\Users\\enT0Xx\\Documents\\mapgen\\graphics\\";
const QString extension = ".png";


map_cell::map_cell(QGraphicsPixmapItem* rel_it){
    this->possible_cell_types = all_tile_options;
    this->neighbours = {};
    this->related_item = rel_it;
    this->uncollapsed = true;
    this->cell_type = "uncollapsed";
    srand(time(0));
}

void map_cell::set_possible_cell_types(std::vector<QString> possible_cell_types){
    this->possible_cell_types = possible_cell_types;
}

std::vector<QString> map_cell::get_possible_cell_types(void){
    return this->possible_cell_types;
}

bool map_cell::is_uncollapsed(void){
    return this->uncollapsed;
}

void map_cell::add_neighbour(map_cell* n){
    this->neighbours.push_back(n);
}

bool is_in(std::vector<QString>& vec, QString elem){
    for (QString& e: vec){
        if (e == elem){return true;}
    }
    return false;
}

bool map_cell::remove_unavailable_types(std::vector<QString>& neighbour_available_types){
    bool shrinked = false;
    int min_border_elem= 10;
    int max_border_elem = -2;
    std::vector<QString> types_to_leave = neighbour_available_types;
    for (QString neighbour_type: neighbour_available_types){
        auto it = find(all_tile_options.begin(), all_tile_options.end(), neighbour_type);
        int pos = static_cast<int>(std::distance(all_tile_options.begin(), it));
        if (pos > max_border_elem){ max_border_elem = pos; }
        if (pos < min_border_elem){min_border_elem = pos;}
    }

    if ((min_border_elem - 1) > -1){types_to_leave.push_back(all_tile_options[min_border_elem - 1]);}
    if ((max_border_elem + 1) < all_tile_options.size()){types_to_leave.push_back(all_tile_options[max_border_elem + 1]);}

    std::vector<int> indexes_to_del = {};
    for (int i = 0; i < this->possible_cell_types.size(); i++){
        if (!is_in(types_to_leave, this->possible_cell_types[i])){
            indexes_to_del.push_back(i);
            shrinked = true;
        }
    }
    int amt_deleted = 0;
    for (int& ind: indexes_to_del){
        this->possible_cell_types.erase(this->possible_cell_types.begin() + (ind - amt_deleted) );
        amt_deleted++;
    }

    return shrinked;
}

int map_cell::get_uncollapsed_options_amt(){
    if (this->uncollapsed){
        int amt = static_cast<int>(this->possible_cell_types.size());
        return amt;
    } else {return 999; }

}

void map_cell::ripple(){
    for (map_cell* n: this->neighbours){
        bool changed = n->remove_unavailable_types(this->possible_cell_types);
        if (changed){
            n->ripple();
        }
    }
}

void map_cell::collapse(){
    if(this->possible_cell_types.empty()){
        std::exit(-1);
    }

    //uodate current cell
    int options_amt = static_cast<int>(this->possible_cell_types.size());
    int final_state_ind = rand() % options_amt;
    QString final_state = this->possible_cell_types[final_state_ind];
    this->possible_cell_types = {final_state};
    this->cell_type = final_state;
    QString final_state_dir = path + final_state + extension;
    QPixmap image(final_state_dir);
    this->related_item->setPixmap(image);
    this->uncollapsed = false;
    //change available cell tiles for all uncollapsed neighbours

    this->ripple();

}

std::vector<map_cell*> potential_next_cells(std::vector<std::vector<map_cell*>>& map_mtrx){
    std::vector<map_cell*> res = {};
    int min_op_amt = 999;
    for (std::vector<map_cell*>& row: map_mtrx){
        for (map_cell* cell: row){
            int op_amt = cell->get_uncollapsed_options_amt();
            if (op_amt < min_op_amt){min_op_amt = op_amt;}
        }
    }

    if (min_op_amt == 999){return {};}

    for (std::vector<map_cell*>& row: map_mtrx){
        for (map_cell* cell: row){
            if (cell->get_uncollapsed_options_amt() == min_op_amt){res.push_back(cell);}
        }
    }

    return res;
}

void collapse_all_mtrx(std::vector<std::vector<map_cell*>>& map_mtrx, int map_row_amt, int map_col_amt){
    srand(time(0));
    std::vector<map_cell*> pot_next = {};

    const int centers_amt = 200;

    int cur_el = 0;

    for (int i = 0; i < centers_amt; i++){
        printf("Cur elem %d\n", cur_el++);
        int row = rand() % map_row_amt;
        int col = rand() % map_col_amt;
        map_mtrx[row][col]->collapse();
    }

    while(true){
        pot_next = potential_next_cells(map_mtrx);
        if (pot_next.size() == 0){break; }
        int rand_ind = rand() % static_cast<int>(pot_next.size());
        map_cell* rand_cell = pot_next[rand_ind];
        printf("Cur elem %d\n", cur_el++);
        rand_cell->collapse();
    }

    return;
}