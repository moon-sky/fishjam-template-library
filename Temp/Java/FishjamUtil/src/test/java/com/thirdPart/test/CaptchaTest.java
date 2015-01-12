package com.thirdPart.test;

/***********************************************************************************************************************************************
 * 验证码(captcha -- Completely Automated Public Test to tell Computers and Humans Apart: 全自动区分计算机和人类的图灵测试 )
 *    实现方案:
 *      1.JCaptcha -- <com.octo.captcha><jcaptcha>[1.0] , 提供了很多种方式(text, sound, image)和实现，并且可以扩展。
 *         实现了若干引擎和组件，通过配置这些引擎和组件，可以方便的修改自己程序captcha构件的算法。
 *         在抵御恶意访问时，可以不用改变代码，灵活快速的改变captcha策略，从而更好的保护系统。
 *      2.patchca -- https://code.google.com/p/patchca/
 *      
 *  使用 -- 通常需要和spring整合，把jcaptcha的服务和引擎还有组件配置成spring的bean
 *   
 *   代码分析：
 *   JCaptcha -- (com.octo.captcha):
 *      1.验证码服务(.service) -- GenericManageableCaptchaService
 *      2.引擎(engine) -- GenericCaptchaEngine
 *      3.生成器 
 *        3.1.文字生成器(component.word.wordgenerator) -- RandomWordGenerator 
 *        3.2.图片生成器(component.image.wordtoimage) -- ComposedWordToImage
 *        3.3.字体生成器(component.image.fontgenerator) -- RandomFontGenerator
 *        3.4.背景生成器(component.image.backgroundgenerator) -- GradientBackgroundGenerator
 *     4.TextPaster
 *        4.1 ??? (component.image.textpaster) -- SimpleTextPaster
 ***********************************************************************************************************************************************/

/***********************************************************************************************************************************************
 * Spring 中将 JCaptcha 定义为 Bean 的简单示例: 
<bean id="imageCaptchaService" class="com.octo.captcha.service.image.DefaultManageableImageCaptchaService">
    <description>验证码服务</description>
    <constructor-arg index="0" type="com.octo.captcha.service.captchastore.CaptchaStore"><ref bean="fastHashMapCaptchaStore"/></constructor-arg>     
    <constructor-arg index="1" type="com.octo.captcha.engine.CaptchaEngine"><ref bean="captchaEngineEx"/></constructor-arg>     
    <constructor-arg index="2"><value>180</value></constructor-arg>                <!-- 超时时间， 秒-->
    <constructor-arg index="3"><value>100000</value></constructor-arg>     	      <!-- 最大并发数 -->
    <constructor-arg index="4"><value>75000</value></constructor-arg>             <!-- TODO: ? -->    
</bean>  
<bean id="fastHashMapCaptchaStore" class="com.octo.captcha.service.captchastore.FastHashMapCaptchaStore"/>     
<bean id="captchaEngineEx" class="com.huachou.yhb.tools.CaptchaEngineEx"/>
 ***********************************************************************************************************************************************/

public class CaptchaTest {

}
