#include <visualizer/home_run_derby_app.h>

using home_run_derby::visualizer::HomeRunDerbyApp;

void prepareSettings(HomeRunDerbyApp::Settings* settings) {
  settings->setResizable(false);
}

CINDER_APP(HomeRunDerbyApp, ci::app::RendererGl, prepareSettings);