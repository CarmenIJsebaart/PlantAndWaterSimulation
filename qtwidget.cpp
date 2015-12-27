#include "qtwidget.h"

#include <sstream>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

#include "ui_qtwidget.h"

std::tuple<double,double> get_min_max(const grid& g) {
  const int height = g.get_height();
  const int width = g.get_width();
  assert(width > 0);
  assert(height > 0);
  std::tuple<double,double> minmax =
    std::make_tuple(
      g.get(0,0),g.get(0,0)
    )
  ;
  for (int y=0; y!=height; ++y) {
    for (int x=0; x!=width; ++x) {
      const double z = g.get(x,y);
      if (z < std::get<0>(minmax)) std::get<0>(minmax) = z;
      if (z > std::get<1>(minmax)) std::get<1>(minmax) = z;
    }
  }
  return minmax;
}

double get_max(const grid& g) {
  const int height = g.get_height();
  const int width = g.get_width();
  assert(width > 0);
  assert(height > 0);
  double max = g.get(0,0);

  for (int y=0; y!=height; ++y) {
    for (int x=0; x!=width; ++x) {
      const double z = g.get(x,y);
      if (z > max) max = z;
    }
  }
  return max;
}

double get_min(const grid& g) {
  const int height = g.get_height();
  const int width = g.get_width();
  assert(width > 0);
  assert(height > 0);
  double min = g.get(0,0);

  for (int y=0; y!=height; ++y) {
    for (int x=0; x!=width; ++x) {
      const double z = g.get(x,y);
      if (z < min) min = z;
    }
  }
  return min;
}

QtWidget::QtWidget(
  QWidget *parent
)
  : QWidget(parent),
    ui(new Ui::QtWidget),
    m_pixmap{},
    m_simulation{}
{
  ui->setupUi(this);

  const int width = m_simulation.get_plant_grid().get_width();
  const int height = m_simulation.get_plant_grid().get_height();
  m_pixmap = QPixmap(width,height);

  //Start a timer
  {
    QTimer * const timer{new QTimer(this)};
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
    timer->setInterval(0);
    timer->start();
  }
}

QtWidget::~QtWidget()
{
  delete ui;
}

void QtWidget::OnTimer()
{
  static int z = 0;

  const int n_repeats = 1;
  for (int i=0; i!=n_repeats; ++i) {
    ++z;
    m_simulation.goto_next_timestep();
  }


  const int height{m_simulation.get_plant_grid().get_height()};
  const int width{m_simulation.get_plant_grid().get_width()};

  QImage image(width,height,QImage::Format_RGB32);
  const grid& plant_grid = m_simulation.get_plant_grid();
  const grid& water_grid = m_simulation.get_water_grid();
  const double min_plants = get_min(plant_grid);
  //const double min_plants = 0.0;
  const double max_plants = get_max(plant_grid) == 0.0 ? 1.0 : get_max(plant_grid);
  //const double max_plants = 20.0;
  assert(min_plants < max_plants);
  const double plants_range = max_plants - min_plants;
  assert(plants_range > 0.0);
  const double min_water = 0.0;
  //const double max_water = get_max(water_grid) == 0.0 ? 1.0 : get_max(water_grid);
  const double max_water = 1.0;
  assert(min_water < max_water);
  const double water_range = max_water - min_water;
  assert(water_range > 0.0);

  {
    std::stringstream s;
    s << "Simulation at time "
      << m_simulation.get_current_time()
      << " (min_plants: " << min_plants
      << ", max_plants: " << max_plants
      << ", min_water: " << min_water
      << ", max_water: " << max_water
      << ", frame: " << z
      << ")"
    ;
    this->setWindowTitle(s.str().c_str());

  }

  for (int y=0; y!=height; ++y)
  {
    for (int x=0; x!=width; ++x)
    {
      const double p = plant_grid.get(x,y);
      const double f_g = ((p - min_plants) / plants_range);
      const int g = std::min(255,static_cast<int>(f_g * 255.0));
      const double w = water_grid.get(x,y);
      const double f_w = ((w - min_water) / water_range);
      const int b = std::min(255,static_cast<int>(f_w * 255.0));
      image.setPixel(x,y,qRgb(0,g,b));
    }
  }
  m_pixmap = QPixmap::fromImage(image);
  update();
}

void QtWidget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawPixmap(
    this->rect(),
    m_pixmap
  );
}
