#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#define GLFW_INCLUDE_VULKAN

#include<GLFW/glfw3.h>
#include<vulkan/vulkan.h>

#define ValidationLayerCount 1

bool CheckValidationLayerSupport(const char *ValidationLayers[])
{
    uint32_t LayerCount;
    vkEnumerateInstanceLayerProperties(&LayerCount, NULL);

    VkLayerProperties AvailableLayers[LayerCount];
    vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers);

    for(int ValidationLayerIndex = 0;
            ValidationLayerIndex < ValidationLayerCount;
            ++ValidationLayerIndex)
    {
        bool LayerFound = false;
        for(int LayerIndex = 0;
                LayerIndex < LayerCount;
                ++LayerIndex)
        {

            VkLayerProperties Layer = AvailableLayers[LayerIndex];
            if(strcmp(ValidationLayers[ValidationLayerIndex], Layer.layerName) == 0)
            {
                LayerFound = true;
                break;
            }
        }
        if(!LayerFound)
        {
            return(false);
        }
    }

    return(true);
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow *Window = glfwCreateWindow(1240, 720, "Vulkan", NULL, NULL);

    const char *ValidationLayers[ValidationLayerCount];
    ValidationLayers[0] = "VK_LAYER_KHRONOS_validation";

#ifdef NDEBUG
    const bool EnableValidationLayer = true;
#else
    const bool EnableValidationLayer = false;
#endif

    VkInstance Instance;
    VkApplicationInfo AppInfo = {};
    AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    AppInfo.pApplicationName = "Vulkan";
    AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    AppInfo.pEngineName = "No Engine";
    AppInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo CreateInfo = {};
    CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    CreateInfo.pApplicationInfo = &AppInfo;

    uint32_t GlfwExtensionCount = 0;
    const char **GlfwExtensions;

    GlfwExtensions = glfwGetRequiredInstanceExtensions(&GlfwExtensionCount);
    CreateInfo.enabledExtensionCount = GlfwExtensionCount;
    CreateInfo.ppEnabledExtensionNames = GlfwExtensions;

    if(EnableValidationLayer && !CheckValidationLayerSupport(ValidationLayers))
    {
        printf("Validation Layers Not Found\n");
        exit(1);
    }
    else
    {
        printf("Validation Layers Found\n");
    }

    if(EnableValidationLayer)
    {
        CreateInfo.enabledLayerCount = (uint32_t)ValidationLayerCount;
        CreateInfo.ppEnabledLayerNames = ValidationLayers;
    }
    else
    {
        CreateInfo.enabledLayerCount = 0;
    }

    VkResult result = vkCreateInstance(&CreateInfo, NULL, &Instance);
    if(result != VK_SUCCESS)
    {
        printf("Failed to create a vulkan instance\n");
        exit(1);
    }

    uint32_t ExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &ExtensionCount, NULL);
    printf("%i Extensions Supported\n", ExtensionCount);
    if(ExtensionCount == 0)
    {
        printf("No Supported Extensions\n");
        exit(1);
    }

    VkExtensionProperties Extensions[ExtensionCount];
    vkEnumerateInstanceExtensionProperties(NULL, &ExtensionCount, Extensions);
    // for(int ExtensionIndex = 0;
    //         ExtensionIndex < ExtensionCount;
    //         ++ExtensionIndex)
    // {
    //     VkExtensionProperties Extension = Extensions[ExtensionIndex];
    //     printf("%s\n", Extension.extensionName);
    // }


    while(!glfwWindowShouldClose(Window))
    {
        glfwPollEvents();
        glfwSwapBuffers(Window);
    }

    printf("Hello vulkan\n");
    return(0);
}
