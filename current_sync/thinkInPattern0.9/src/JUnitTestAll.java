
import junit.framework.*;
import junit.swingui.*;

public class JUnitTestAll {
  public static void main(String[] args) {
    new TestRunner().start(new String[] { "JUnitTestAll" });
  }
  public static Test suite() { 
    TestSuite suite = new TestSuite(); 
    suite.addTest(new TestSuite(simplifying.MessengerDemo.class));
    suite.addTest(new TestSuite(simplifying.CollectingParameterDemo.class));
    suite.addTest(new TestSuite(singleton.SingletonPattern.class));
    suite.addTest(new TestSuite(singleton.ConnectionPoolDemo.class));
    suite.addTest(new TestSuite(proxy.ProxyDemo.class));
    suite.addTest(new TestSuite(proxy.ConnectionPoolProxyDemo.class));
    suite.addTest(new TestSuite(state.KissingPrincess.class));
    suite.addTest(new TestSuite(state.KissingPrincess2.class));
    suite.addTest(new TestSuite(state.StateDemo.class));
    suite.addTest(new TestSuite(strategy.StrategyPattern.class));
    suite.addTest(new TestSuite(templatemethod.TemplateMethod.class));
    suite.addTest(new TestSuite(factory.Games.class));
    suite.addTest(new TestSuite(factory.shapefact1.ShapeFactory1.class));
    suite.addTest(new TestSuite(factory.shapefact2.ShapeFactory2.class));
    suite.addTest(new TestSuite(builder.BuildMedia.class));
    suite.addTest(new TestSuite(decorator.nodecorators.CoffeeShop.class));
    suite.addTest(new TestSuite(decorator.alldecorators.CoffeeShop2.class));
    suite.addTest(new TestSuite(decorator.compromise.CoffeeShop3.class));
    suite.addTest(new TestSuite(adapter.SimpleAdapter.class));
    suite.addTest(new TestSuite(adapter.AdapterVariations.class));
    suite.addTest(new TestSuite(bridge.BridgeStructure.class));
    suite.addTest(new TestSuite(composite.CompositeStructure.class));
    suite.addTest(new TestSuite(observer.ObservedFlower.class));
    suite.addTest(new TestSuite(facade.Facade.class));
    suite.addTest(new TestSuite(command.CommandPattern.class));
    suite.addTest(new TestSuite(chainofresponsibility.FindMinima.class));
    suite.addTest(new TestSuite(multipledispatch.PaperScissorsRock.class));
    suite.addTest(new TestSuite(visitor.BeeAndFlowers.class));
    suite.addTest(new TestSuite(statemachine.mousetrap1.MouseTrapTest.class));
    suite.addTest(new TestSuite(statemachine.mousetrap2.MouseTrap2Test.class));
    suite.addTest(new TestSuite(statemachine.vendingmachine.VendingMachineTest.class));
    suite.addTest(new TestSuite(refactor.recyclea.RecycleA.class));
    suite.addTest(new TestSuite(refactor.recycleap.RecycleAP.class));
    suite.addTest(new TestSuite(refactor.recycleb.RecycleB.class));
    suite.addTest(new TestSuite(refactor.doubledispatch.DoubleDispatch.class));
    suite.addTest(new TestSuite(refactor.trashvisitor.TrashVisitor.class));
    suite.addTest(new TestSuite(refactor.dynatrash.DynaTrash.class));

    return suite;
  }
}
