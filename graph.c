// #define TURTLE_IMPLEMENTATION
#include "turtle.h"
#include <time.h>

typedef struct {
    char filename[4096];
    list_t *content;
    list_t *graphMinimums;
    list_t *graphMaximums;
    double graphLeftX;
    double graphRightX;
    double graphBottomY;
    double graphTopY;
    int32_t xTicks;
} graph_t;

graph_t self;

void init() {
    self.content = list_init();
    self.graphMaximums = list_init();
    list_append(self.graphMaximums, (unitype) 0.0, 'd');
    self.graphMinimums = list_init();
    list_append(self.graphMinimums, (unitype) 0.0, 'd');
    self.graphLeftX = -200;
    self.graphRightX = 200;
    self.graphBottomY = -140;
    self.graphTopY = 150;
    self.xTicks = 10;
}

int32_t import(char *filename) {
    strcpy(self.filename, filename);
    list_free(self.content);
    self.content = osToolsLoadCSVDouble(self.filename, OSTOOLS_CSV_ROW);
    if (self.content -> length < 1) {
        return -1;
    }
    list_delete(self.content, 0);
    /* sort graph by X field (todo) */

    /* get top and bottom bounds */
    self.graphMinimums -> data[0].d = self.content -> data[0].r -> data[0].d;
    self.graphMaximums -> data[0].d = self.content -> data[self.content -> length - 1].r -> data[0].d;
    for (int32_t i = 0; i < self.content -> length; i++) {
        list_t *dataPoint = self.content -> data[i].r;
        while (dataPoint -> length > self.graphMaximums -> length) {
            list_append(self.graphMinimums, (unitype) 100000000000.0, 'd');
            list_append(self.graphMaximums, (unitype) -100000000000.0, 'd');
        }
        for (int32_t j = 1; j < dataPoint -> length; j++) {
            if (dataPoint -> data[j].d < self.graphMinimums -> data[j].d) {
                self.graphMinimums -> data[j].d = dataPoint -> data[j].d;
            }
            if (dataPoint -> data[j].d > self.graphMaximums -> data[j].d) {
                self.graphMaximums -> data[j].d = dataPoint -> data[j].d;
            }
        }
    }
    list_print(self.graphMaximums);
    list_print(self.graphMinimums);
    return 0;
}

void render() {
    if (self.content -> length < 1) {
        return;
    }
    /* render data */
    turtlePenSize(1);
    int32_t mouseIndex = -1;
    double minMouseX = 1000;
    tt_setColor(TT_COLOR_RED);
    double xScale = (self.graphRightX - self.graphLeftX) / (self.graphMaximums -> data[0].d - self.graphMinimums -> data[0].d);
    double yScale = (self.graphTopY - self.graphBottomY) / (self.graphMaximums -> data[1].d - self.graphMinimums -> data[1].d);
    for (int32_t i = 0; i < self.content -> length; i++) {
        double xValue = (self.content -> data[i].r -> data[0].d - self.graphMinimums -> data[0].d) * xScale + self.graphLeftX;
        double yValue = (self.content -> data[i].r -> data[1].d - self.graphMinimums -> data[1].d) * yScale + self.graphBottomY;
        turtleGoto(xValue, yValue);
        if (i == 0) {
            turtlePenDown();
        }
        if (fabs(xValue - turtle.mouseX) < minMouseX) {
            mouseIndex = i;
            minMouseX = fabs(xValue - turtle.mouseX);
        }
        // printf("%lf %lf\n", xValue, yValue);
    }
    turtlePenUp();
    /* render graph */
    double pensizehalf = 1.5;
    tt_setColor(TT_COLOR_TEXT);
    turtlePenSize(pensizehalf * 2);
    turtleGoto(self.graphLeftX - pensizehalf, self.graphTopY + pensizehalf);
    turtlePenDown();
    turtleGoto(self.graphLeftX - pensizehalf, self.graphBottomY - pensizehalf);
    turtleGoto(self.graphRightX + pensizehalf, self.graphBottomY - pensizehalf);
    turtlePenUp();
    turtlePenSize(pensizehalf);
    for (int32_t i = 0; i < self.xTicks; i++) {
        double tickX = self.graphLeftX - pensizehalf + (self.graphRightX - self.graphLeftX + pensizehalf * 2) / (self.xTicks - 1) * i;
        turtleGoto(tickX, self.graphBottomY - 2);
        turtlePenDown();
        turtleGoto(tickX, self.graphBottomY - 6);
        turtlePenUp();
        double adjustedXScale = (self.graphRightX - self.graphLeftX + pensizehalf * 2) / (self.graphMaximums -> data[0].d - self.graphMinimums -> data[0].d);
        turtleTextWriteStringf(tickX, self.graphBottomY - pensizehalf - 12, 8, 50, "%.0lf", (tickX - self.graphLeftX + pensizehalf) / adjustedXScale);
    }
    turtleTextWriteStringf(self.graphLeftX - pensizehalf - 5, self.graphBottomY - pensizehalf + 3, 8, 100, "%.3lfuW", self.graphMinimums -> data[1].d);
    turtleTextWriteStringf(self.graphLeftX - pensizehalf - 5, self.graphTopY + pensizehalf - 3, 8, 100, "%.3lfuW", self.graphMaximums -> data[1].d);
    turtleTextWriteString("Seconds", (self.graphLeftX + self.graphRightX) / 2, self.graphBottomY - pensizehalf - 30, 8, 50);
    turtleTextWriteStringf(self.graphLeftX - pensizehalf, self.graphBottomY - pensizehalf - 12, 8, 50, "%.0lf", self.graphMinimums -> data[0].d - self.graphMinimums -> data[0].d);
    turtleTextWriteStringf(self.graphRightX + pensizehalf, self.graphBottomY - pensizehalf - 12, 8, 50, "%.0lf", self.graphMaximums -> data[0].d - self.graphMinimums -> data[0].d);
    /* render mouse */
    turtlePenSize(5);
    if (turtle.mouseX > self.graphLeftX - pensizehalf * 2 && turtle.mouseX < self.graphRightX + pensizehalf * 2 && turtle.mouseY > self.graphBottomY && turtle.mouseY < self.graphTopY) {
        if (mouseIndex >= 0 && mouseIndex < self.content -> length) {
            double xValue = (self.content -> data[mouseIndex].r -> data[0].d - self.graphMinimums -> data[0].d) * xScale + self.graphLeftX;
            double yValue = (self.content -> data[mouseIndex].r -> data[1].d - self.graphMinimums -> data[1].d) * yScale + self.graphBottomY;
            turtleGoto(xValue, yValue);
            turtlePenDown();
            turtlePenUp();
            double xDisplay = self.content -> data[mouseIndex].r -> data[0].d - self.graphMinimums -> data[0].d;
            turtleRectangle(xValue + 5, yValue + 4, xValue + 5 + 4 + turtleTextGetStringLengthf(8, "%.0lf, %.3lfuW", xDisplay, self.content -> data[mouseIndex].r -> data[1].d), yValue + 16);
            tt_setColor(TT_COLOR_BACKGROUND);
            turtleTextWriteStringf(xValue + 7, yValue + 10, 8, 0, "%.0lf, %.3lfuW", xDisplay, self.content -> data[mouseIndex].r -> data[1].d);
        }
    }
}

void parseRibbonOutput() {
    if (tt_ribbon.output[0] == 0) {
        return;
    }
    tt_ribbon.output[0] = 0;
    if (tt_ribbon.output[1] == 0) { // File
        if (tt_ribbon.output[2] == 1) { // New
            printf("New\n");
        }
        if (tt_ribbon.output[2] == 2) { // Save
            if (osToolsFileDialog.selectedFilenames -> length == 0) {
                if (osToolsFileDialogSave(OSTOOLS_FILE_DIALOG_FILE, "", NULL) != -1) {
                    printf("Saved to: %s\n", osToolsFileDialog.selectedFilenames -> data[0].s);
                }
            } else {
                printf("Saved to: %s\n", osToolsFileDialog.selectedFilenames -> data[0].s);
            }
        }
        if (tt_ribbon.output[2] == 3) { // Save As...
            if (osToolsFileDialogSave(OSTOOLS_FILE_DIALOG_FILE, "", NULL) != -1) {
                printf("Saved to: %s\n", osToolsFileDialog.selectedFilenames -> data[0].s);
            }
        }
        if (tt_ribbon.output[2] == 4) { // Open
            if (osToolsFileDialogOpen(OSTOOLS_FILE_DIALOG_SINGLE_SELECT, OSTOOLS_FILE_DIALOG_FILE, "", NULL) != -1) {
                import(osToolsFileDialog.selectedFilenames -> data[0].s);
                printf("Loaded data from: %s\n", osToolsFileDialog.selectedFilenames -> data[0].s);
            }
        }
    }
    if (tt_ribbon.output[1] == 1) { // Edit
        if (tt_ribbon.output[2] == 1) { // Undo
            printf("Undo\n");
        }
        if (tt_ribbon.output[2] == 2) { // Redo
            printf("Redo\n");
        }
        if (tt_ribbon.output[2] == 3) { // Cut
            osToolsClipboardSetText("test123");
            printf("Cut \"test123\" to clipboard!\n");
        }
        if (tt_ribbon.output[2] == 4) { // Copy
            osToolsClipboardSetText("test345");
            printf("Copied \"test345\" to clipboard!\n");
        }
        if (tt_ribbon.output[2] == 5) { // Paste
            osToolsClipboardGetText();
            printf("Pasted \"%s\" from clipboard!\n", osToolsClipboard.text);
        }
    }
    if (tt_ribbon.output[1] == 2) { // View
        if (tt_ribbon.output[2] == 1) { // Change theme
            printf("Change theme\n");
            if (tt_theme == TT_THEME_DARK) {
                turtleBgColor(36, 30, 32);
                turtleToolsSetTheme(TT_THEME_COLT);
            } else if (tt_theme == TT_THEME_COLT) {
                turtleBgColor(212, 201, 190);
                turtleToolsSetTheme(TT_THEME_NAVY);
            } else if (tt_theme == TT_THEME_NAVY) {
                turtleBgColor(255, 255, 255);
                turtleToolsSetTheme(TT_THEME_LIGHT);
            } else if (tt_theme == TT_THEME_LIGHT) {
                turtleBgColor(30, 30, 30);
                turtleToolsSetTheme(TT_THEME_DARK);
            }
        } 
        if (tt_ribbon.output[2] == 2) { // GLFW
            printf("GLFW settings\n");
        } 
    }
}

int main(int argc, char *argv[]) {
    /* Initialise glfw */
    if (!glfwInit()) {
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // MSAA (Anti-Aliasing) with 4 samples (must be done before window is created (?))

    /* Create a windowed mode window and its OpenGL context */
    const GLFWvidmode *monitorSize = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int32_t windowHeight = monitorSize -> height;
    GLFWwindow *window = glfwCreateWindow(windowHeight * 16 / 9, windowHeight, "turtle demo", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeLimits(window, windowHeight * 16 / 9 * 0.4, windowHeight * 0.4, windowHeight * 16 / 9, windowHeight);

    /* initialise turtle */
    turtleInit(window, -320, -180, 320, 180);
    glfwSetWindowSize(window, windowHeight * 16 / 9 * 0.85, monitorSize -> height * 0.85); // doing it this way ensures the window spawns in the top left of the monitor and fixes resizing limits
    /* initialise turtleText */
    turtleTextInit("config/roberto.tgl");
    /* initialise turtleTools ribbon */
    turtleToolsSetTheme(TT_THEME_DARK); // dark theme preset
    ribbonInit("config/ribbonConfig.txt");
    /* initialise osTools */
    osToolsInit(argv[0], window); // must include argv[0] to get executableFilepath, must include GLFW window
    osToolsFileDialogAddGlobalExtension("txt"); // add txt to extension restrictions

    init();
    if (argc > 1) {
        import(argv[1]);
    }

    uint32_t tps = 120; // ticks per second (locked to fps in this case)
    uint64_t tick = 0; // count number of ticks since application started
    clock_t start, end;

    while (turtle.close == 0) {
        start = clock();
        turtleGetMouseCoords();
        turtleClear();
        render();
        turtleToolsUpdate(); // update turtleTools
        parseRibbonOutput(); // user defined function to use ribbon
        turtleUpdate(); // update the screen
        end = clock();
        while ((double) (end - start) / CLOCKS_PER_SEC < (1.0 / tps)) {
            end = clock();
        }
        tick++;
    }
    turtleFree();
    glfwTerminate();
    return 0;
}