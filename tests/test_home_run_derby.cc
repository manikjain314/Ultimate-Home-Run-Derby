#include <core/ball.h>
#include <core/bat.h>
#include <core/canvas_frame.h>
#include <visualizer/simulator.h>

#include <catch2/catch.hpp>

using ci::Color;
using glm::vec2;
using home_run_derby::Ball;
using home_run_derby::Bat;
using home_run_derby::CanvasFrame;
using home_run_derby::visualizer::Simulator;
using std::pair;

TEST_CASE("Test Ball class") {
  Ball ball(1, 5, 0.6f, 0.1f, 0.2f, 1, 25, 2, 2, 3, 3, 100);
  ball.SetGroundLocation(80);

  SECTION("Test updating states and colliding with ground") {
    for (size_t i = 0; i < 6; ++i) {
      REQUIRE(ball.GetSpeed().y <= 0);
      ball.UpdateStates();
    }

    for (size_t i = 0; i < 12; ++i) {
      REQUIRE(ball.GetSpeed().y >= 0);
      ball.UpdateStates();
    }

    REQUIRE(Approx(ball.GetSpeed().x).epsilon(0.01) == 1.8f);
    REQUIRE(Approx(ball.GetSpeed().y).epsilon(0.01) == -0.84f);
    REQUIRE(Approx(ball.GetPosition().x).epsilon(0.01) == 30.8f);
    REQUIRE(Approx(ball.GetPosition().y).epsilon(0.01) == 79.16f);
  }

  SECTION("Test resetting states") {
    for (size_t i = 0; i < 5; ++i) {
      ball.UpdateStates();
    }
    REQUIRE(ball.GetPosition() != vec2(-5, 50));
    ball.ResetState();
    REQUIRE(ball.GetPosition() == vec2(-5, 50));
    REQUIRE(ball.GetSpeed() == vec2(2, -3));
  }

  SECTION("Test gravity") {
    float initial_velocity = ball.GetSpeed().y;
    ball.UpdateStates();
    REQUIRE(ball.GetSpeed().y == initial_velocity + 0.6f);
    REQUIRE(ball.GetPosition() == vec2(-3, 47));
  }

  SECTION("Test restitution") {
    for (size_t i = 0; i < 17; ++i) {
      ball.UpdateStates();
    }
    float initial_velocity = ball.GetSpeed().y;
    ball.UpdateStates();
    REQUIRE(ball.GetSpeed().y == -0.2f * initial_velocity + 0.6f);
    REQUIRE(Approx(ball.GetPosition().x).epsilon(0.01) == 30.8f);
    REQUIRE(Approx(ball.GetPosition().y).epsilon(0.01) == 79.16f);
  }

  SECTION("Test friction") {
    ball.UpdateStates();
    float initial_velocity = ball.GetSpeed().x;
    for (size_t i = 0; i < 19; ++i) {
      ball.UpdateStates();
    }
    REQUIRE(ball.GetSpeed().x == 0.9f * initial_velocity);
    REQUIRE(Approx(ball.GetPosition().x).epsilon(0.01) == 34.4f);
    REQUIRE(Approx(ball.GetPosition().y).epsilon(0.01) == 78.08f);
  }

  SECTION("Test terminal velocity") {
    Ball terminal_ball(1, 5, 1, 0.1f, 0.2f, 0.1f, 1, 2, 2, 3, 3, 1000);
    terminal_ball.SetGroundLocation(970);
    for (size_t i = 0; i < 30; ++i) {
      terminal_ball.UpdateStates();
    }
    REQUIRE(terminal_ball.GetSpeed().y == 1);
  }

  SECTION("Test QuadraticSolver()") {
    pair<float, float> solution = ball.QuadraticSolver(1, -6, 8);
    REQUIRE(solution.first == 4);
    REQUIRE(solution.second == 2);
  }

  SECTION("Test colliding with bat, linear") {
    Bat bat(1, 1);
    bat.SetBatPosition(vec2(-4.5, 50));
    bat.SetBatSpeed(vec2(-2, 0));
    ball.UpdateStates();
    ball.HandleBatCollisions(bat);
    REQUIRE(Approx(ball.GetSpeed().x).epsilon(0.001) == 0.0392f);
    REQUIRE(Approx(ball.GetSpeed().y).epsilon(0.001) == -3.532f);
  }

  SECTION("Test colliding with bat, non-linear") {
    Bat bat(1, 1);
    bat.SetBatPosition(vec2(-4.5, 50));
    bat.SetBatSpeed(vec2(-2, -1));
    ball.UpdateStates();
    ball.HandleBatCollisions(bat);
    REQUIRE(Approx(ball.GetSpeed().x).epsilon(0.001) == 1.633f);
    REQUIRE(Approx(ball.GetSpeed().y).epsilon(0.001) == -3.051f);
  }

  SECTION("Test colliding with bat, bat x speed = 0") {
    Bat bat(1, 1);
    bat.SetBatPosition(vec2(-4.5, 50));
    bat.SetBatSpeed(vec2(0, -1));
    ball.UpdateStates();
    ball.HandleBatCollisions(bat);
    REQUIRE(Approx(ball.GetSpeed().x).epsilon(0.001) == 1.293f);
    REQUIRE(Approx(ball.GetSpeed().y).epsilon(0.001) == -0.515f);
  }

  SECTION("Test colliding with bat, bat end point overlaps") {
    Bat bat(1, 1);
    bat.SetBatPosition(vec2(-5, 50));
    bat.SetBatSpeed(vec2(-10, -1));
    ball.HandleBatCollisions(bat);
    REQUIRE(Approx(ball.GetSpeed().x).epsilon(0.001) == -9.683f);
    REQUIRE(Approx(ball.GetSpeed().y).epsilon(0.001) == -4.168f);
  }

  SECTION("Test colliding with bat, bat end point goes past") {
    Bat bat(1, 1);
    bat.SetBatPosition(vec2(-10, 50));
    bat.SetBatSpeed(vec2(-20, -5));
    ball.HandleBatCollisions(bat);
    REQUIRE(Approx(ball.GetSpeed().x).epsilon(0.001) == -16.645f);
    REQUIRE(Approx(ball.GetSpeed().y).epsilon(0.001) == -11.972f);
  }

  SECTION("Test colliding with bat twice") {
    Bat bat(1, 1);
    bat.SetBatPosition(vec2(-1, 50));
    bat.SetBatSpeed(vec2(-2, 0));
    ball.HandleBatCollisions(bat);
    ball.UpdateStates();
    bat.SetBatPosition(vec2(-6, 47));
    bat.SetBatSpeed(vec2(-10, 47));
    REQUIRE(Approx(ball.GetSpeed().x).epsilon(0.001) == -2);
    REQUIRE(Approx(ball.GetSpeed().y).epsilon(0.001) == -2.4);
  }
}

TEST_CASE("Test CanvasFrame class") {
  CanvasFrame canvas(10, 1080, 16.0f / 9.0f, 30, 2, 3, 5, 1);

  SECTION("Test CalculateCharacterHeadLocation() without offset") {
    canvas.CalculateCharacterHeadLocation(vec2(0, 0));
    REQUIRE(canvas.GetPlayerHeadLocation() == vec2(1900, 1040));
  }

  SECTION("Test CalculateCharacterBodyLocation() without offset") {
    canvas.CalculateCharacterBodyLocation(vec2(0, 0));
    REQUIRE(canvas.GetPlayerBodyLocation() == vec2(1900, 1025));
  }

  SECTION("Test CalculateGroundLocation() without offset") {
    canvas.CalculateGroundLocation(vec2(0, 0));
    REQUIRE(canvas.GetGroundLocation().first == vec2(0, 1050));
    REQUIRE(canvas.GetGroundLocation().second == vec2(1920, 1080));
  }

  SECTION("Test CalculateDirtLocation() without offset") {
    canvas.CalculateDirtLocation(vec2(0, 0));
    REQUIRE(canvas.GetDirtLocation().first == vec2(0, 1080));
    REQUIRE(canvas.GetDirtLocation().second == vec2(1920, 1080));
  }

  SECTION("Test UpdateCanvas() without offset") {
    canvas.UpdateCanvas(vec2(0, 0), vec2(0, 0));
    REQUIRE(canvas.GetPlayerHeadLocation() == vec2(1900, 1040));
    REQUIRE(canvas.GetPlayerBodyLocation() == vec2(1900, 1025));
    REQUIRE(canvas.GetGroundLocation().first == vec2(0, 1050));
    REQUIRE(canvas.GetGroundLocation().second == vec2(1920, 1080));
  }

  SECTION("Test CalculateCharacterHeadLocation() with offset") {
    canvas.CalculateCharacterHeadLocation(vec2(10, 20));
    REQUIRE(canvas.GetPlayerHeadLocation() == vec2(1910, 1060));
  }

  SECTION("Test CalculateCharacterBodyLocation() with offset") {
    canvas.CalculateCharacterBodyLocation(vec2(10, 20));
    REQUIRE(canvas.GetPlayerBodyLocation() == vec2(1910, 1045));
  }

  SECTION("Test CalculateGroundLocation() with offset") {
    canvas.CalculateGroundLocation(vec2(10, 20));
    REQUIRE(canvas.GetGroundLocation().first == vec2(0, 1070));
    REQUIRE(canvas.GetGroundLocation().second == vec2(1920, 1100));
  }

  SECTION("Test CalculateDirtLocation() with offset") {
    canvas.CalculateDirtLocation(vec2(10, 20));
    REQUIRE(canvas.GetDirtLocation().first == vec2(0, 1100));
    REQUIRE(canvas.GetDirtLocation().second == vec2(1920, 1080));
  }

  SECTION("Test UpdateCanvas() with offset") {
    canvas.UpdateCanvas(vec2(10, 20), vec2(0, 0));
    REQUIRE(canvas.GetPlayerHeadLocation() == vec2(1910, 1060));
    REQUIRE(canvas.GetPlayerBodyLocation() == vec2(1910, 1045));
    REQUIRE(canvas.GetGroundLocation().first == vec2(0, 1070));
    REQUIRE(canvas.GetGroundLocation().second == vec2(1920, 1100));
  }

  SECTION("Test UpdateCanvas() with velocity offset") {
    canvas.UpdateCanvas(vec2(10, 20), vec2(2, 3));
    REQUIRE(canvas.GetPlayerHeadLocation() == vec2(1910, 1060));
    REQUIRE(canvas.GetPlayerBodyLocation() == vec2(1910, 1045));
    REQUIRE(canvas.GetGroundLocation().first == vec2(0, 1070));
    REQUIRE(canvas.GetGroundLocation().second == vec2(1920, 1100));
  }
}

TEST_CASE("Test Simulator class") {
  Simulator simulator(10, 1080, 16.0f / 9.0f, 30, 10, 10, 0.8f, 0.2f, 0.3f, 1,
                      25, 5, 5, 2, 2, 10, 5, 1, 2, 5, 4);
  SECTION("Test UpdateOffset()") {
    REQUIRE(simulator.GetCanvasFrame().GetGroundLocation().first ==
            vec2(0, 1050));
    REQUIRE(simulator.GetCanvasFrame().GetGroundLocation().second ==
            vec2(1920, 1080));
    simulator.UpdateOffset(vec2(10, 20));
    REQUIRE(simulator.GetCanvasFrame().GetGroundLocation().first ==
            vec2(0, 1070));
    REQUIRE(simulator.GetCanvasFrame().GetGroundLocation().second ==
            vec2(1920, 1100));
  }

  SECTION("Test UpdateBallStates()") {
    REQUIRE(simulator.GetBall().GetSpeed() == vec2(5, -2));
    REQUIRE(simulator.GetBall().GetPosition() == vec2(-10, 540));
    simulator.UpdateBallStates();
    REQUIRE(simulator.GetBall().GetSpeed() == vec2(5, -1.2));
    REQUIRE(simulator.GetBall().GetPosition() == vec2(-5, 538));
  }

  SECTION("Test UpdateBatStates()") {
    simulator.UpdateBatStates(vec2(10, 20));
    REQUIRE(simulator.GetBat().GetBatSpeed() == vec2(10, 20));
    REQUIRE(simulator.GetBat().GetBatPosition() == vec2(10, 20));
    simulator.UpdateBatStates(vec2(50, 40));
    REQUIRE(simulator.GetBat().GetBatSpeed() == vec2(40, 20));
    REQUIRE(simulator.GetBat().GetBatPosition() == vec2(50, 40));
  }

  SECTION("Test ResetStates()") {
    simulator.UpdateBallStates();
    simulator.UpdateBallStates();
    simulator.ResetStates();
    REQUIRE(simulator.GetBall().GetSpeed() == vec2(5, -2));
    REQUIRE(simulator.GetBall().GetPosition() == vec2(-10, 540));
  }

  SECTION("Test ResetGame()") {
  }

  SECTION("Test IncrementGameState()") {
    REQUIRE(simulator.GetCurrentGameState() == 0);
    simulator.IncrementGameState();
    REQUIRE(simulator.GetCurrentGameState() == 1);
    simulator.IncrementGameState();
    REQUIRE(simulator.GetCurrentGameState() == 2);
    simulator.IncrementGameState();
    REQUIRE(simulator.GetCurrentGameState() == 0);
  }

  SECTION("Test outs for ball not colliding with bat") {
    REQUIRE(simulator.GetOuts() == 0);
    for (size_t i = 0; i < 50; ++i) {
      simulator.UpdateBallStates();
    }
    REQUIRE(simulator.GetOuts() == 1);
  }

  SECTION("Test outs for ball colliding with bat but staying in screen") {
    REQUIRE(simulator.GetOuts() == 0);
    for (size_t i = 0; i < 9; ++i) {
      simulator.UpdateBallStates();
    }
    simulator.UpdateBatStates(vec2(35.9f, 535.16f));
    simulator.UpdateBallStates();
    simulator.UpdateBatStates(vec2(35.75f, 535.18f));

    for (size_t i = 0; i < 50; ++i) {
      simulator.UpdateBallStates();
    }
    REQUIRE(simulator.GetOuts() == 2);
  }

  SECTION("Test outs for ball colliding with bat and leaving screen") {
    REQUIRE(simulator.GetOuts() == 0);
    for (size_t i = 0; i < 9; ++i) {
      simulator.UpdateBallStates();
    }
    simulator.UpdateBatStates(vec2(35.9f, 535.16f));
    simulator.UpdateBallStates();
    simulator.UpdateBatStates(vec2(10, 535.18f));

    for (size_t i = 0; i < 50; ++i) {
      simulator.UpdateBallStates();
    }
    REQUIRE(simulator.GetOuts() == 0);
  }
}