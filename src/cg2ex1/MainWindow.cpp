#include "MainWindow.h"
#include <tbd/log.h>

MainWindow::MainWindow(QMainWindow *parent) : QMainWindow(parent)
{
  setupUi(this);

  connect(btnRadius,SIGNAL(clicked()), this, SLOT(setSelectionMode()));
  connect(btnKNearest,SIGNAL(clicked()), this, SLOT(setSelectionMode()));
  connect(spinKNearest,SIGNAL(editingFinished()), this, SLOT(setKNearest()));
  connect(spinRadius,SIGNAL(editingFinished()), this, SLOT(setRadius()));
  connect(spinPointSize,SIGNAL(editingFinished()), this, SLOT(setPointSize()));
  connect(spinKNearest,SIGNAL(valueChanged(int)), this, SLOT(setKNearest()));
  connect(spinVertexID,SIGNAL(valueChanged(int)), this, SLOT(setVertexId()));
  connect(spinRadius,SIGNAL(valueChanged(double)), this, SLOT(setRadius()));
  connect(spinPointSize,SIGNAL(valueChanged(double)), this, SLOT(setPointSize()));
  connect(btnRenderKDTree,SIGNAL(clicked()), this, SLOT(setDrawKDTree()));

  updateConfig();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setPointSize()
{
  glWidget->pointSize_ = spinPointSize->value();
  glWidget->update();
}

void MainWindow::setRadius()
{
  glWidget->radius_ = spinRadius->value();
  setVertexId();
}

void MainWindow::setDrawKDTree()
{
//  glWidget->pointCloud.drawKDTree(btnRenderKDTree->isChecked());
  glWidget->update();
}

void MainWindow::setKNearest()
{
  glWidget->kNearest_ = spinKNearest->value();
  setVertexId();
}

void MainWindow::setVertexId()
{
//  int value = spinVertexID->value();
//  if (value < 0) value = 0;
//  if (value > (int)glWidget->pointCloud.size()) value = (int)glWidget->pointCloud.size();

//  glWidget->selection = glWidget->pointCloud[value].v;

//  if (glWidget->selectionMode == GLWidget::SELECT_RADIUS) 
//    glWidget->pointCloud.collectInRadius(glWidget->selection,glWidget->radius); else
//  if (glWidget->selectionMode == GLWidget::SELECT_KNEAREST)
//    glWidget->pointCloud.collectKNearest(glWidget->selection,glWidget->kNearest);
  
  glWidget->update();
}

void MainWindow::setSelectionMode()
{
  glWidget->selectionMode_ = (btnRadius->isChecked()) ? GLWidget::SELECT_RADIUS : GLWidget::SELECT_KNEAREST;
  setVertexId();
}

void MainWindow::updateConfig()
{
  glWidget->config_.drawObjects_ = checkObjects->isChecked();
  glWidget->config_.drawCamera_ = checkCamera->isChecked();
  glWidget->config_.drawLight_ = checkLight->isChecked();
  glWidget->config_.drawLabels_ = checkLabels->isChecked();
  glWidget->config_.drawCoords_ = checkCoords->isChecked();
  glWidget->config_.drawAxis_ = checkAxis->isChecked();
  glWidget->config_.lockLight_ = checkLockLight->isChecked();
  glWidget->update();
}

