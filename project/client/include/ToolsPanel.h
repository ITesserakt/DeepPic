//
// Created by eremey on 18.11.2021.
//
#pragma once
#include <QToolBar>

class ToolsPanel : public QToolBar {
    Q_OBJECT

public:
    explicit ToolsPanel(QWidget *parent = 0);
    ~ToolsPanel() = default;
};

