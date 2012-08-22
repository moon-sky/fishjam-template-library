#ifndef RDF_STUDY_H
#define RDF_STUDY_H

/*******************************************************************************************************
* RDF(Resource Description Framework) -- 资源描述框架,描述资源之间关系用的一种模型，通常使用XML格式存储。
*   RDF 使用三元组(Triple) 方法描述 资源(resource) 和 文本(literal) 构成的世界中个体的关系，
*   语法为：主语(或主题)、谓语(或谓词)和宾语(或目标)。其中 主语与宾语就是资源。而谓语是两个资源之间的关系。
*   如 <http://www.xulplanet.com/rdf/people/Sandra>  =>  name =>     Sandra 
*            resource(使用URI的表示方法)                 关系     literal(文本)
*
* RDF内置的关系谓词
*   rdf:alt  -- 定义了多个备选值，但应该只有一个可用
*   rdf:bag  -- 无序表
*   rdf:Description -- 描述了一个三元组
*   rdf:files -- 文件，使用 file:///xxxx 的形式( 如 : file:///d:/Firefox/ )?
*   rdf:resource -- 定义的是目标resource
*   rdf:seq  -- 顺序列表
*   rdf:type -- 表示实例与类之间的关系
*******************************************************************************************************/



#endif //RDF_STUDY_H