package us.ihmc.concurrent.runtime.barrierScheduler.implicitContext.tasks;

import us.ihmc.concurrent.runtime.barrierScheduler.implicitContext.Task;

/**
 * Minimal implementation of a task that uses {@link BindingContext}
 */
public abstract class CopyableContextTask extends Task<BindingContext>
{
   public CopyableContextTask(long divisor)
   {
      super(divisor);
   }

   @Override
   protected void updateMasterContext(BindingContext context)
   {
      context.updateMasterContext();
   }

   @Override
   protected void updateLocalContext(BindingContext context)
   {
      context.updateLocalContext();
   }
}
