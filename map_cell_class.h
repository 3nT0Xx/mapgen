#ifndef MAP_CELL_CLASS_H
#define MAP_CELL_CLASS_H
#include <vector>
#include <QGraphicsItem>
#include <Qstring>


class map_cell
{
private:
    std::vector<map_cell*> neighbours;
    std::vector<QString> possible_cell_types;
    QGraphicsPixmapItem* related_item;
    bool uncollapsed;
    QString cell_type;
public:
    map_cell(QGraphicsPixmapItem* rel_it);

    void set_possible_cell_types(std::vector<QString> possible_cell_types);

    std::vector<QString> get_possible_cell_types(void);

    bool is_uncollapsed(void);

    void add_neighbour(map_cell* n);

    bool remove_unavailable_types(std::vector<QString>& neighbour_available_types);

    void collapse(void);

    void ripple(void);

    int get_uncollapsed_options_amt();
};


void collapse_all_mtrx(std::vector<std::vector<map_cell*>>& map_mtrx, int map_row_amt, int map_col_amt);
#endif // MAP_CELL_CLASS_H

