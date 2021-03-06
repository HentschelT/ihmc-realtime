package us.ihmc.realtime;

import us.ihmc.affinity.CPUTopology;
import us.ihmc.affinity.Package;
import us.ihmc.concurrent.runtime.barrierScheduler.implicitContext.BarrierScheduler;
import us.ihmc.concurrent.runtime.barrierScheduler.implicitContext.tasks.BindingContext;
import us.ihmc.concurrent.runtime.barrierScheduler.implicitContext.tasks.CopyableContextTask;
import us.ihmc.concurrent.runtime.barrierScheduler.implicitContext.tasks.InPlaceCopyable;

import java.util.Arrays;
import java.util.List;
import java.util.Random;

/**
 * @author Doug Stephen <a href="mailto:dstephen@ihmc.us">(dstephen@ihmc.us)</a>
 */
public class TestBarrierSchedulerCyclic
{
   private static final long SCHEDULER_PERIOD_NANOSECONDS = 500000;
   private static final int NUM_ITERATIONS_OF_SCHEDULER = 120000;
   private static final double ESTIMATED_DURATION = (double) SCHEDULER_PERIOD_NANOSECONDS * (double) NUM_ITERATIONS_OF_SCHEDULER / 1e9;

   public static class TestCyclicContext extends BindingContext
   {
      public TestCyclicContext(ExamineVariablesTask examineVariablesTask, UpdateVariablesTask updateVariablesTask)
      {
         bind(updateVariablesTask.cyclicData, examineVariablesTask.cyclicData);
      }
   }

   private static class TimingInformation
   {
      long previousTime = 0;
      long avgJitter = 0;
      long maxJitter = 0;

      long periodInNS;
      long iterations = 0;

      TimingInformation(String name, long periodInNS)
      {
         this.periodInNS = periodInNS;

         System.out.println(name + " Period, Hz: " + 1 / (periodInNS / 1e9));
      }

      public void initialize(long currentTime)
      {
         previousTime = currentTime;
      }

      public void updateTimingInformation(long newTime)
      {
         long jitter = Math.abs(newTime - previousTime - periodInNS);

         if (jitter > maxJitter)
         {
            maxJitter = jitter;
         }

         previousTime = newTime;
         avgJitter += jitter;

         iterations++;
      }

      public double getFinalMaxJitterMicroseconds()
      {
         return (double) maxJitter / 1e3;
      }

      public double getFinalAvgJitterMicroseconds()
      {
         return (double) avgJitter / (double) iterations / 1e3;
      }
   }

   public static class TestCyclicData implements InPlaceCopyable<TestCyclicData>
   {
      private final int[] someInts = new int[16];
      private final long[] someLongs = new long[8];
      private final float[] someFloats = new float[16];
      private final double[] someDoubles = new double[8];

      @Override
      public void copyFrom(TestCyclicData src)
      {
         System.arraycopy(src.someInts, 0, someInts, 0, someInts.length);

         System.arraycopy(src.someLongs, 0, someLongs, 0, someLongs.length);

         System.arraycopy(src.someFloats, 0, someFloats, 0, someFloats.length);

         System.arraycopy(src.someDoubles, 0, someDoubles, 0, someDoubles.length);
      }
   }

   private static class UpdateVariablesTask extends CopyableContextTask
   {
      private final Random random = new Random(1976L);
      private final TestCyclicData cyclicData = new TestCyclicData();
      private final TimingInformation timingInformation;

      boolean firstTick = true;

      public UpdateVariablesTask(long divisor)
      {
         super(divisor);

         timingInformation = new TimingInformation("Update Task", SCHEDULER_PERIOD_NANOSECONDS * divisor);
      }

      public void doReporting()
      {
         System.out.format("Update Task Jitter: avg = %.4f us, max = %.4f us%n", timingInformation.getFinalAvgJitterMicroseconds(),
                           timingInformation.getFinalMaxJitterMicroseconds());
      }

      /**
       * Initializes the internal state of the task.
       * <p>
       * Called once, immediately before the first {@link #execute()} call. This method is executed on
       * the task's thread.
       */
      @Override
      protected boolean initialize()
      {
         return true;
      }

      /**
       * Executes a single iteration of this task.
       * <p>
       * This method is executed on the task's thread.
       */
      @Override
      protected void execute()
      {
         for (int i = 0; i < cyclicData.someInts.length; i++)
         {
            cyclicData.someInts[i] = random.nextInt();
         }

         for (int i = 0; i < cyclicData.someLongs.length; i++)
         {
            cyclicData.someLongs[i] = random.nextLong();
         }

         for (int i = 0; i < cyclicData.someFloats.length; i++)
         {
            cyclicData.someFloats[i] = random.nextFloat();
         }

         for (int i = 0; i < cyclicData.someDoubles.length; i++)
         {
            cyclicData.someDoubles[i] = random.nextDouble();
         }

         if (firstTick)
         {
            timingInformation.initialize(System.nanoTime());
            firstTick = false;
         }
         else
         {
            timingInformation.updateTimingInformation(System.nanoTime());
         }
      }

      /**
       * Perform any cleanup before shutting down. Called the next tick after a
       * shutdown request from the barrier scheduler.
       */
      @Override
      protected void cleanup()
      {

      }
   }

   private static class ExamineVariablesTask extends CopyableContextTask
   {
      final Random random = new Random(1976L);
      final TimingInformation timingInformation;
      final TestCyclicData cyclicData = new TestCyclicData();

      private int anInt;
      private long aLong;
      private float aFloat;
      private double aDouble;

      boolean firstTick = true;

      public ExamineVariablesTask(long divisor)
      {
         super(divisor);

         timingInformation = new TimingInformation("Examine Task", SCHEDULER_PERIOD_NANOSECONDS * divisor);
      }

      public void doReporting()
      {
         System.out.format("Examine Task Jitter: avg = %.4f us, max = %.4f us%n", timingInformation.getFinalAvgJitterMicroseconds(),
                           timingInformation.getFinalMaxJitterMicroseconds());
      }

      /**
       * Initializes the internal state of the task.
       * <p>
       * Called once, immediately before the first {@link #execute()} call. This method is executed on
       * the task's thread.
       */
      @Override
      protected boolean initialize()
      {
         return true;
      }

      /**
       * Executes a single iteration of this task.
       * <p>
       * This method is executed on the task's thread.
       */
      @Override
      protected void execute()
      {
         anInt = (int) (cyclicData.someInts[random.nextInt(cyclicData.someInts.length)] * random.nextDouble());
         aLong = (long) (cyclicData.someLongs[random.nextInt(cyclicData.someLongs.length)] * random.nextDouble());
         aFloat = (float) (cyclicData.someFloats[random.nextInt(cyclicData.someFloats.length)] * random.nextDouble());
         aDouble = cyclicData.someDoubles[random.nextInt(cyclicData.someDoubles.length)] * random.nextDouble();

         if (firstTick)
         {
            timingInformation.initialize(System.nanoTime());
            firstTick = false;
         }
         else
         {
            timingInformation.updateTimingInformation(System.nanoTime());
         }
      }

      /**
       * Perform any cleanup before shutting down. Called the next tick after a
       * shutdown request from the barrier scheduler.
       */
      @Override
      protected void cleanup()
      {

      }
   }

   public static void main(String[] args) throws InterruptedException
   {
      UpdateVariablesTask updateVariablesTask = new UpdateVariablesTask(2);
      ExamineVariablesTask examineVariablesTask = new ExamineVariablesTask(10);

      PriorityParameters updateTaskPriority = new PriorityParameters(95);
      PriorityParameters examineVariablesPriority = new PriorityParameters(95);

      RealtimeThread examineThread = new RealtimeThread(examineVariablesPriority, examineVariablesTask, "examineTask");
      RealtimeThread updateThread = new RealtimeThread(updateTaskPriority, updateVariablesTask, "updateTask");

      Package cpuPackage = new CPUTopology().getPackage(0);

      CPUDMALatency.setLatency(0);

      System.out.println("Starting cyclic test [Iterations: " + NUM_ITERATIONS_OF_SCHEDULER + "; Estimated Duration: " + ESTIMATED_DURATION + "s]");

      System.out.println("Pinning examine thread to core 2 and update thread to core 3.");
      examineThread.setAffinity(cpuPackage.getCore(2).getDefaultProcessor());
      updateThread.setAffinity(cpuPackage.getCore(3).getDefaultProcessor());

      examineThread.start();
      updateThread.start();

      List<CopyableContextTask> tasks = Arrays.asList(examineVariablesTask, updateVariablesTask);

      TestCyclicContext context = new TestCyclicContext(examineVariablesTask, updateVariablesTask);

      BarrierScheduler<BindingContext> barrierScheduler = new BarrierScheduler<>(tasks, context, BarrierScheduler.TaskOverrunBehavior.SKIP_TICK);

      PriorityParameters schedulerPriority = new PriorityParameters(99);
      PeriodicParameters periodicParameters = new PeriodicParameters(SCHEDULER_PERIOD_NANOSECONDS);

      final TimingInformation schedulerTimingInformation = new TimingInformation("Scheduler", SCHEDULER_PERIOD_NANOSECONDS);

      RealtimeThread schedulerThread = new RealtimeThread(schedulerPriority, periodicParameters, "barrierSchedulerThread")
      {
         boolean firstTick = true;
         int iterations = -1;
         @Override
         public void run()
         {
            while (iterations < NUM_ITERATIONS_OF_SCHEDULER)
            {
               super.waitForNextPeriod();
               if (firstTick)
               {
                  schedulerTimingInformation.initialize(System.nanoTime());
                  firstTick = false;
               }
               else
               {
                  schedulerTimingInformation.updateTimingInformation(System.nanoTime());
               }

               barrierScheduler.run();
               iterations++;
            }

            barrierScheduler.shutdown();

            updateVariablesTask.doReporting();
            examineVariablesTask.doReporting();

            System.out.format("Scheduler Jitter: avg = %.4f us, max = %.4f us%n", schedulerTimingInformation.getFinalAvgJitterMicroseconds(),
                              schedulerTimingInformation.getFinalMaxJitterMicroseconds());
         }
      };

      System.out.println("Pinning scheduler thread to core 1");
      schedulerThread.setAffinity(cpuPackage.getCore(1).getDefaultProcessor());
      schedulerThread.start();
      schedulerThread.join();

      System.out.println("Thread done??");
   }
}
