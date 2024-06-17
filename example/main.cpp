#include <cstdlib>
#include <format>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <rubus-gui/screen.hpp>
#include <rubus-gui/renderer.hpp>
#include <rubus-gui/tree.hpp>

auto ui_screen = rugui::Screen{800, 600};
auto ui_renderer = rugui::SkiaRenderer{};
auto ui_tree = rugui::Tree{};

auto main() -> int {
  glfwSetErrorCallback([](int error, const char *description) {
    std::cerr << std::format("GLFW Error [{}]: {}\n", error, description);
  });

  if (!glfwInit()) {
    std::cerr << "GLFW init failed\n";
    return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

  auto window = glfwCreateWindow(ui_screen.width, ui_screen.height, "Rubus GUI", nullptr, nullptr);
  if (!window) {
    std::cerr << "GLFW create window failed\n";
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  ui_renderer.init(&ui_screen);
  ui_tree.init(&ui_screen);

  glfwSetFramebufferSizeCallback(window, [](GLFWwindow *, int width, int height) {
    ui_screen.set_size(width, height);
    ui_renderer.regenerate_surface(&ui_screen);
    ui_tree.set_size(&ui_screen);
  });

  glfwSetCursorEnterCallback(window, [](GLFWwindow *window, int entered) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int x = std::floor(xpos);
    int y = std::floor(ypos);
    if (entered) {
      ui_tree.run_mouse_event(x, y);
    } else {
      ui_tree.run_mouse_leave_window_event(x, y);
    }
  });

  glfwSetCursorPosCallback(window, [](GLFWwindow *, double xpos, double ypos) {
    int x = std::floor(xpos);
    int y = std::floor(ypos);
    ui_tree.run_mouse_event(x, y);
  });

  glfwSetScrollCallback(window, [](GLFWwindow *, double, double yoffset) {
    ui_tree.run_vscroll_event((int)(yoffset * 25));
  });

  auto skimg_spellbook = SkImages::DeferredFromEncodedData(SkData::MakeFromFileName("./example/spellbook.png"));

  ui_tree.root
    ->add((new rugui::Node{"title", "Hello, 세상!"})
            ->set_font_size(30)
            ->set_color(SkColors::kBlue)
            ->set_flex_self_align(rugui::FlexAlign::Center))
    ->add((new rugui::Node{"row"})
            ->set_flex_dir(rugui::FlexDir::Col)
            ->set_width(rugui::Size::Parent(1))
            ->set_height(rugui::Size::FitContent())
            ->add((new rugui::Node{"image"})
                    ->set_width(rugui::Size::Self(32 * 2))
                    ->set_height(rugui::Size::Self(32 * 2))
                    ->set_image(skimg_spellbook)
                    ->set_image_sampling(SkSamplingOptions{SkFilterMode::kNearest}))
            ->add((new rugui::Node{"box"})
                    ->set_color(SkColor4f::FromColor(0xFF'EAEAEA))
                    ->set_flex_self_align(rugui::FlexAlign::Center)
                    ->set_padding(10)
                    ->set_width(rugui::Size::Parent(1))
                    ->set_height(rugui::Size::FitContent())
                    ->add((new rugui::Node{
                      "text",
                      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut "
                      "labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco "
                      "laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in "
                      "voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat "
                      "non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."})))
            ->add((new rugui::Node{"box"})
                    ->set_color(SkColors::kGray)
                    ->set_flex_self_align(rugui::FlexAlign::Center)
                    ->set_width(rugui::Size::Parent(1))
                    ->set_height(rugui::Size::FitContent())
                    ->add((new rugui::Node{
                      "text",
                      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut "
                      "labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco "
                      "laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in "
                      "voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat "
                      "non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."}))));

  while (!glfwWindowShouldClose(window)) {
    ui_renderer.clear(SkColors::kWhite);
    ui_tree.root->layout(&ui_renderer);
    ui_tree.root->draw_all(&ui_renderer);
    ui_renderer.flush();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  ui_renderer.~SkiaRenderer();
  glfwDestroyWindow(window);

  glfwTerminate();
  return EXIT_SUCCESS;
}
