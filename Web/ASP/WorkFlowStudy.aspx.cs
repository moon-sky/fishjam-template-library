using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Threading;

//精通C# 3.0与.NET 3.5高级编程 -- 第19章 WF编程入门 
//http://download.csdn.net/detail/liukang1919/5024787#comment -- ASP.NET与工作流WF实现审批流程
/************************************************************************************************************
 * VS2008 中有专门的向导，名称空间为 System.Workflow.*;
 * 
 * WWF(Windows Workflow Foundation) -- .NET 3.0 中提供的工作流开发框架，提供了一套用于辅助工作流开发的框架，被
 *   宿主到独立应用程序中(如 WindowsForm程序、ASP.NET 应用程序等)，允许开发人员提取应用程序逻辑，配合VS的设计工具，
 *   让开发人员提前设计好程序流程，在代码中只需关心实现细节。
 *   流程：宿主程序创建 WorkflowRuntime 类的实例，开启工作流引擎，订阅和公开WorkflowRuntime事件来监视每个工作流实例的进度，
 *         默认情况下，WorkflowRuntime以异步的形式执行工作流，
 * 
 * 在一个应用程序域中可以有多个工作流运行时引擎，并且运行时引擎的每个实例均可支持多个并发运行的工作流实例.
 * 
 * WF设计视图
 *   提供了各种活动，如 while活动， code活动,
 * 
 * 特点
 *   1.WF具有强大和灵活的工作流开发能力，使用Visual Studio提供的可视化设计工具和WF的运行时引擎，让开发人员能够以一种统一的方式开发应用程序。
 *   2.WF支持顺序工作流、状态机工作流和数据驱动工作流类型，并且支持工作流的持久化，这对于一些需要一天甚至一年之后再继续的工作流来说非常有用
 *   3.WF支持使用特定领域模型解决问题，开发人员可以开发自己的活动组件，来引入对于特定领域问题的解决方法
 *   4.WF提供了完整的工作流开发系统，除了工作流运行时本身，微软也提供了一套标准的活动、工作流持久化、工作流监控和追踪、
 *     规则引擎、与VS紧密整合的工作流设计器，以及项目开发模板等
 *   5.可以修改WF的默认行来扩展WF。比如，可以提供自定义的持久化服务来取代WF提供的SQL持久化服务
 *   6.WF包含在Visual Studio中，无需花费额外成本
 * 
 * 工作流类型，设计时有 Document Outline 工具( View -> Other Windows -> Document Outline )
 *   1.SequentialWorkflowActivity -- 顺序工作流，定义了一系列按照指定步骤执行的步骤。
 *     ExcuteCode
 *   2.StateMachineWorkflowActivit --  状态机工作流，没有定义固定顺序的步骤，状态集合和在状态之间的状态转换。
 *     StateActivity是状态机工作流的主要活动，加入后可以添加 EventDrivenActivity、StateInitializationActivity 等各种活动。
 *     SetState -- 设置工作流状态，转移到指定状态
 *   3.??? -- 基于顺序工作流的规则驱动工作流
 * 
 * 工作流创建模式：
 *   1.仅限代码 -- 这是WF的默认创作模式。
 *   2.代码分离 -- 此模式下，可以通过使用工作流标记XAML，并将其与C#或Visual Basic代码隐藏实现进行组合来定义工作流，
 *                 .xoml(标记语言以序列化后的形式保存工作流模型) + .xoml.cs()
 *   3.无代码 -- 此模式下，可以通过使用工作流标记来创作工作流。然后可以使用WF命令行工作流编译器来编译工作流。
 *   
 * 需要的机制：持久化、补偿、跟踪机制
 * 
 * 相关类
 *   System.WorkflowServices.* -- 提供了工作流与WCF服务进行整合的能力
 *   WorkflowRuntime -- 控制工作流运行时引擎，开启一个独立的工作流任务，可以为该类关联一些事件来监视工作流运行时的运行。
 *     能跟踪和使用可插接的服务来关联当前的执行环境。
 *     方法
 *       AddService -- 将指定的服务添加到工作流运行时引擎中
 *       CreateWorkflow -- 使用指定参数创建工作流实例(WorkflowInstance)
 *       GetAllServices -- 检索实现或派生自指定的Type且已添加到工作流运行时引擎的所有服务
 *       GetLoadedWorkflows -- 获取一个集合，该集合包含当前加载到内存的所有工作流实例
 *       StartRuntime -- 启动工作流运行时引擎和工作流运行时引擎服务。
 *     事件(很多)
 *       Started -- 启动工作流运行引擎时发生
 *       Stopped -- 工作流引擎停止后发生
 *       WorkflowAborted -- 在终止工作流实例时发生
 *       WorkflowCompleted -- 在完成工作流实例时发生
 *       WorkflowCreated -- 在创建工作流实例后发生
 *       WorkflowLoaded -- 将工作流实例加载到内存时发生
 *       WorkflowTerminated -- 在终止工作流实例时发生
 *       
 *   WorkflowInstance -- 一个单独的可执行的工作流实例，担当着实际工作流的代理类，用户可以通过访问该类来获知当前工作流的执行情况
 *     InstanceId -- 获取工作流实例的全局唯一标识符GUID值
 *     Unload -- 将工作流实例从内存卸载到持久性存储区。此调用将停止工作流的执行，直至当前计划的工作完成或事务范围结束。
 *     GetWorkflowDefinition -- 检索此工作流实例最顶层的根活动，即返回一个Activity类型的对象
 *     
 * Activity -- 即活动表示工作流中的一个步骤。活动是工作流的基本单元，所有的活动都直接或间接派生自System.Workflow.ComponentModel.Activity类
 *   IfElseBranchActivity 
 *   CodeActivity -- 允许开发人员编写一段将用于执行的程序代码
 *   
 * Condition(条件) -- 驱动工作流活动的执行行为
 *   Code(代码) -- 用户可以指定一个方法名。在该方法中使用代码配置条件，并返回条件的结果，通过 ConditionalEventArgs.Result 设置执行哪个分支
 *   Declarative Rule Condition(声明性规则条件) -- 使用规则定义条件。可以利用VS 2008的规则设计器来定义规则
 *   
 * 创建自定义的活动 -- 支持领域模型
 *
 * 为工作流传递参数 -- CreateWorkflow方法接收一个 Dictionary 的参数,
 *   Dictionary对象的键使用string类型，通常是在工作流实例中定义的公共属性的名称(会自动映射)，对应的值使用简单的Object对象，
 * 
 * WF的运行时引擎支持可扩展服务(应用程序启动时注册服务到运行时引擎中)
 *   核心服务 -- 由Microsoft预先定义，如 持久化服务(),
 *     文件持久化服务：FilePersistenceService
 *     SQL持久化服务：SqlWorkflowPersistenceServices
 *     线程服务() -- 开发者可以控制工作流实例的执行方式，默认是异步执行，通过 ASPNetThreadingService 可以改成适应Web的同步方式？
 *     其他的有 ? 跟踪服务、外部数据服务、时间服务、事务服务等 ?
 *   本地服务 -- 由开发人员定义并实现
 *   
 * 在WWF中，开发者可以利用或创建"运行时刻服务"来监控甚至修改该工作流运行时刻。
 *   持续性服务-存储执行和空闲时间之间的工作流状态
 *   追踪服务-输出有关工作流执行的信息到某种媒体
 *   事务服务-帮助维持工作流执行过程中的数据完整性
 *
************************************************************************************************************/

public partial class WorkFlowStudy : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {

    }
}

#if NO_COMPILE

public partial class Workflow1 : SequentialWorkflowActivity
{     
    private bool isRight;
    public bool IsRight
    {
        get { return isRight; }
        set { isRight = value; }
    }
    //响应CodeActivity的ExcuteCode事件来执行一段自定义的代码
    private void codeActivity1_ExecuteCode(object sender, EventArgs e)
    {
        string answer = Console.ReadLine();
        if (answer == "祸从口出") 
        {
            Console.WriteLine("恭喜！回答正确，程序将退出");
            isRight = true;
        }
        else
            Console.WriteLine("回答错误，请重新回答");
    }
}

static void Main(string[] args) 
{
    Console.WriteLine("请回答谜底：言多必失！打一成语");
    //创建工作流引擎，并创建一个工作流实例
    using(WorkflowRuntime workflowRuntime = new WorkflowRuntime())
    {
        AutoResetEvent waitHandle = new AutoResetEvent(false);
        //关联工作流引擎的WorkflowCompleted和WorkflowTerminated事件
        workflowRuntime.WorkflowCompleted += delegate(object sender, WorkflowCompletedEventArgs e) {waitHandle.Set();};
        workflowRuntime.WorkflowTerminated += delegate(object sender, WorkflowTerminatedEventArgs e)
        {
            Console.WriteLine(e.Exception.Message);
            waitHandle.Set();
        };
        //创建并开启工作流
        WorkflowInstance instance = workflowRuntime.CreateWorkflow(typeof(AnswerToRiddleWF.Workflow1));
        instance.Start();
        waitHandle.WaitOne();
    }
}

#endif