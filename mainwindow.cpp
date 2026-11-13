#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qlabel.h>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>
#include "map_cell_class.h"

const int map_row_amt = 300;
const int map_col_amt = 300;

std::vector<std::pair<int, int>> generate_all_possible_neighbour_coords(int cur_row, int cur_column)
{
    std::vector<std::pair<int, int>> res = {};
    bool up_available = (cur_row - 1) > -1;
    bool down_available = (cur_row + 1) < map_row_amt;
    bool left_available = (cur_column - 1) > -1;
    bool right_available = (cur_column + 1) < map_col_amt;

    if (up_available){
        res.push_back(std::make_pair(cur_row - 1, cur_column));
        if (left_available){
            res.push_back(std::make_pair(cur_row - 1, cur_column - 1));
        }
        if(right_available){
            res.push_back(std::make_pair(cur_row - 1, cur_column + 1));
        }
    }

    if (down_available){
        res.push_back(std::make_pair(cur_row + 1, cur_column));
        if (left_available){
            res.push_back(std::make_pair(cur_row + 1, cur_column - 1));
        }
        if(right_available){
            res.push_back(std::make_pair(cur_row + 1, cur_column + 1));
        }
    }

    if (left_available){
        res.push_back(std::make_pair(cur_row, cur_column - 1));
    }
    if (right_available){
        res.push_back(std::make_pair(cur_row, cur_column + 1));
    }

    return res;

}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qreal const pic_dim = 20.0;
    ui->setupUi(this);
    QPixmap uncollapsed_image("C:\\Users\\enT0Xx\\Documents\\mapgen\\uncollapsed.png");
    QGraphicsScene* scene = new QGraphicsScene(this);
    QGraphicsView* view = new QGraphicsView();
    std::vector<std::vector<map_cell*>> map_mtrx = std::vector<std::vector<map_cell*>>(map_row_amt, {});
    for (int i = 0; i < map_row_amt * map_col_amt; i++){
        int row = i / map_row_amt;
        int col = i % map_col_amt;
        QGraphicsPixmapItem* uncoll_pix_item = new QGraphicsPixmapItem(uncollapsed_image);
        uncoll_pix_item->setPos(col*pic_dim, row*pic_dim);
        scene->addItem(uncoll_pix_item);
        map_cell* cell = new map_cell(uncoll_pix_item);
        map_mtrx[row].push_back(cell);
    }
    view->setScene(scene);
    this->setCentralWidget(view);

    for (int row_num = 0; row_num < map_row_amt; row_num++){
        for (int col_num = 0; col_num < map_col_amt; col_num++){
            map_cell* cur_cell = map_mtrx[row_num][col_num];
            std::vector<std::pair<int, int>> possible_neighbours = generate_all_possible_neighbour_coords(row_num, col_num);
            for (std::pair<int, int>& coords: possible_neighbours){
                cur_cell->add_neighbour(map_mtrx[coords.first][coords.second]);
            }
        }
    }

    collapse_all_mtrx(map_mtrx, map_row_amt, map_col_amt);
}

MainWindow::~MainWindow()
{   
    delete ui;
}
