package com.fishjam.spring.test.framework;

import javax.servlet.http.HttpSession;

import junit.framework.TestCase;

import org.junit.Test;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.servlet.ModelAndView;


/***********************************************************************************************************************************************
 * 
***********************************************************************************************************************************************/

public class SpringMVCTest extends TestCase {

	@Test
	public void testMVC(){
		assertEquals(1, 1);
	}
	
	@RequestMapping("/login/login.do")
	 public ModelAndView listBuyerProduct(HttpSession session,
			 @RequestParam(required = false, defaultValue = "1" )  Integer curPage
			 )
	{
       ModelAndView modelAndView = new ModelAndView("login");
//       modelAndView.addObject("listavailablebuyerproducts", buyerProductService.listAvailableBuyerProducts());
//       if (session.getAttribute("userinfo") != null) {
//           modelAndView.addObject("myid1", JSONArray.fromObject(quotedpriceOneService.listMyQuotedpriceOneIdByUserId(((User) session.getAttribute("userinfo")).getId())).toString());
//           modelAndView.addObject("myid2", JSONArray.fromObject(quotedpriceTwoService.listMyQuotedpriceTwoIdByUserId(((User) session.getAttribute("userinfo")).getId())).toString());
//       }
       return modelAndView;
   }
}
