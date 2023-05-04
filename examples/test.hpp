#ifndef A46DE159_0278_41AA_A838_F1EBE6D9422E
#define A46DE159_0278_41AA_A838_F1EBE6D9422E

#include <XAD/XAD.hpp>
#include <cmath>
#include <stack>

/// Applies the sin function a number of times.
template <class T>
void repeated_sin(int n, T& x) {
    using std::sin;

    for (int i = 0; i < n; ++i) x = sin(x);
}
/**
 * A callback object that can be inserted into the tape and will be called
 * by XAD's computeAdjoints at the points where it was inserted during
 * the forward run.
 *
 * Its only virtual method is computeAdjoint - the computeForward is added
 * for convenience of implementation.
 */
template <class Tape>
class SinCheckpointCallback : public xad::CheckpointCallback<Tape> {
   public:
    typedef typename Tape::slot_type slot_type;      // type for slot in the tape
    typedef typename Tape::value_type value_type;    // double
    typedef typename Tape::active_type active_type;  // AReal<double>

    /** Computes one stage of the algorithm passively, stores a checkpoint,
     *  and puts the result back into the active data type.
     *
     *  This method is not part of the callback interface and could be
     * implemented standalone alternatively.
     */
    void computeStage(int n, active_type& x) {
        // store data needed for reverse run
        n_.push(n);                // store how many iterations
        slots_.push(x.getSlot());  // store the slot of the input variable
        value_type x_p = value(x);
        x_.push(x_p);  // store passive value of the input

        // run the algorithm passively
        repeated_sin(n, x_p);

        // update the active output value
        value(x) = x_p;
        slots_.push(x.getSlot());  // store the slot of the output variable

        // register callback for the reverse path
        Tape::getActive()->insertCallback(this);
    }

    /** Callback function for the reverse path.
     *
     * This function gets called by XAD during computeAdjoints to update
     * the adjoints with the checkpointed function,
     * computed with XAD in a nested tape in this case.
     */
    void computeAdjoint(Tape* tape) {
        // restore checkpoint variables
        slot_type outputslot = slots_.top();
        slots_.pop();
        slot_type inputslot = slots_.top();
        slots_.pop();
        int n = n_.top();
        n_.pop();

        // retrieve adjoint of output
        // Important: retrieve this before any active operations, as that may
        //            overwrite the value
        value_type outputadj = tape->getAndResetOutputAdjoint(outputslot);

        // create new AD type and start nested recording from after that
        active_type x = x_.top();
        x_.pop();
        tape->registerInput(x);

        // start a nested tape to compute adjoints for just this part
        xad::ScopedNestedRecording<Tape> nested(tape);

        // run actively
        repeated_sin(n, x);

        tape->registerOutput(x);
        derivative(x) = outputadj;  // set output adjoint
        nested.computeAdjoints();   // calculate adjoints in nested recording

        // increment input derivative
        nested.incrementAdjoint(inputslot, derivative(x));
    }

   private:
    // storage for checkpoints
    // (all checkpoints in the same object, hence we use a stack)
    std::stack<int> n_;            // number of iterations for this stage
    std::stack<value_type> x_;     // the input value for this stage
    std::stack<slot_type> slots_;  // slots for the input, then output
};

// driver function that computes the value of x and its adjoint
// note that x_adj is also an input variable with the initial adjoint seed
void repeated_sin_checkpointed(int n, double& x, double& x_adj) {
    // setup of data types
    typedef xad::adj<double> mode;
    typedef mode::tape_type tape_type;
    typedef mode::active_type AD;

    // initialize tape
    tape_type tape;

    AD x_ad = x;  // initialize indepedent variables
    tape.registerInput(x_ad);
    tape.newRecording();  // start recording derivatives

    // setup checkpointing object
    SinCheckpointCallback<tape_type> chkpt;

    int checkpt_distance = 4;  // distance between checkpoints
    using std::min;
    for (int i = 0; i < n; i += checkpt_distance) {
        int m = min(checkpt_distance, n - i);
        chkpt.computeStage(m, x_ad);  // run one stage forward + checkpoint
    }

    tape.registerOutput(x_ad);
    derivative(x_ad) = x_adj;  // set final output derivative
    tape.computeAdjoints();    // compute adjoints of independents

    x_adj = derivative(x_ad);  // set return adjoint
    x     = value(x_ad);       // set return value
}

#endif /* A46DE159_0278_41AA_A838_F1EBE6D9422E */
