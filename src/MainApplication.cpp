#ifndef SOFTVELVET_MAINAPPLICATION_H
#define SOFTVELVET_MAINAPPLICATION_H

#include "JuceHeader.h"
#include "MainComponent.h"

class MainApplication : public juce::JUCEApplication {
 public:
  MainApplication() = default;

  const juce::String getApplicationName() override {
    return ProjectInfo::projectName;
  }

  const juce::String getApplicationVersion() override {
    return ProjectInfo::versionString;
  }

  bool moreThanOneInstanceAllowed() override { return false; }

  void initialise(const juce::String& commandLine) override {
    juce::ignoreUnused(commandLine);
    this->mainWindow.reset(new MainWindow(getApplicationName()));
  }

  void shutdown() override { this->mainWindow = nullptr; }

  void systemRequestedQuit() override { quit(); }

  void anotherInstanceStarted(const juce::String& commandLine) override {
    juce::ignoreUnused(commandLine);
  }

  class MainWindow : public juce::DocumentWindow {
   public:
    explicit MainWindow(juce::String name)
        : DocumentWindow(
              name,
              juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(
                  ResizableWindow::backgroundColourId),
              DocumentWindow::allButtons) {
      setUsingNativeTitleBar(true);
      setContentOwned(new MainComponent(), true);

#if JUCE_IOS || JUCE_ANDROID
      this->setFullScreen(true);
#else
      this->setResizable(false, false);
      this->centreWithSize(this->getWidth(), this->getHeight());
#endif
      this->setVisible(true);
    }

    void closeButtonPressed() override {
      JUCEApplication::getInstance()->systemRequestedQuit();
    }

   private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
  };

 private:
  std::unique_ptr<MainWindow> mainWindow;
};

#endif  // SOFTVELVET_MAINAPPLICATION_H
